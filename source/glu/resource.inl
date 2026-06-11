#pragma once
#include "../lib/gl/gl.hxx"
#include "../tool/integral-allocator.hxx"
#include "../tool/constraints.hxx"
#include "../tool/journal.hxx"
#include <vector>
#include <utility>  // for std::swap and std::move
#include <functional>
#include <mutex>
#include <cassert>

#ifndef RESOURCE_INCLUDED
#define RESOURCE_INCLUDED
#include "resource.hxx"
#undef RESOURCE_INCLUDED
#endif

namespace hegem::glu
{
    inline namespace resource_details
    {
        // can only exists (allocated and destructed) in 1 thread.
        template <class T, class Traits>
        struct resource_pool: tool::non_transferable
        {
            using resource_type = T;
            using traits = Traits;
            using resource_id_type = typename traits::id_type;

            using name_allocator_type = tool::integral_allocator;
            using shared = shared_resource<resource_type, traits>;
            friend shared;

            using name_type = int;
            static constexpr auto null_name() { return 0; }

            // should only be called in gl thread
            shared allocate()
            {
                auto name = name_allocator.allocate();
                if (name == static_cast<name_type>(resources.size())) {
                    auto first_pos = resources.size();
                    auto bulk_size = traits::preferred_bulk_allocation_size();
                    auto size = first_pos + bulk_size;
                    resources.resize(size, null_name());
                    traits::bulk_allocate(bulk_size, &resources[first_pos]);
                    {
                        std::lock_guard<std::mutex> _{m};
                        names_refcounts.resize(size, 0);
                    }
                }
                assert(name < static_cast<name_type>(resources.size()));

                return {*this, name+1, resources[name]};
            }

            // can be called every frame in gl thread
            void try_recycle()
            {
                // TODO: recycle for every frame currently,
                // maybe a bottleneck, should profile though.
                recycle();
            }

            ~resource_pool()
            {
                bool dirty = false;

                auto log = j() << "(dtor)\n";
                for (auto refs: names_refcounts) {
                    log << " " << refs;
                    if (refs > 0) dirty = true;
                }
                if (names_refcounts.size() != 0) log << "\n";

                if (dirty) {
                    log << " \e[1;33mWARNING: \e[0;33mperforming dirty destruction\e[0m\n";
                } else {
                    log << " performing clean destruction\n";
                }

                if (resources.size() > 0)
                    traits::bulk_delete(resources.size(), &resources[0]);
                log << " resource released\n";
            }

            static resource_pool& instance();

        private:
            int pool_id;
            name_allocator_type name_allocator;
            std::vector<resource_id_type> resources;
            std::vector<int> names_refcounts;
            std::vector<name_type> recycling_pending_names;
            std::mutex m;

            resource_pool()
            {
                pool_id = resource_pool_count++;
                resource_recycler::instance()
                    .register_try_recycle_fn([this] () { try_recycle(); });
                j() << "(ctor)\n";
            }

            // may be called from any thread
            name_type ref(name_type name)
            {
                std::lock_guard<std::mutex> _{m};
                names_refcounts[name-1]++;
                return name;
            }

            // may be called from any thread
            void unref(name_type name)
            {
                name--;
                std::lock_guard<std::mutex> _{m};
                assert(names_refcounts[name] > 0);
                if (--names_refcounts[name] == 0)
                    recycling_pending_names.emplace_back(name);
            }

            void recycle()
            {
                std::vector<name_type> names;
                {
                    std::lock_guard<std::mutex> _{m};
                    names = std::move(recycling_pending_names);
                }
                for (auto name: names) {
                    auto log = j() << "recycling " << name << ": <" << (int)resources[name] << ">\n";
                    traits::recycle(resources[name]);
                    log << " recycled for <" << (int)resources[name] << ">\n";
                    name_allocator.deallocate(name);
                }
            }

            auto j() const
            {
                return tool::journal{"POOL"}
                    << "\e[0;36m[" << traits::name() << "|" << pool_id << "]\e[0m ";
            }
        };

        template <class T, class Traits>
        auto resource_pool<T, Traits>::instance() -> resource_pool&
        {
            static resource_pool pool;
            return pool;
        }

        // Copy, move, dtor are all thread-safe
        template <class T, class Traits>
        struct shared_resource
        {
            using pool = resource_pool<T, Traits>;
            using name_type = typename pool::name_type;
            using resource_id_type = typename pool::resource_id_type;
            static constexpr auto null_name() { return pool::null_name(); }
            friend pool;

            shared_resource(): p{}, name{null_name()}, id{0} {}

            // rule of five (yes, there are only 4 functions in this case)
            shared_resource(shared_resource const& other)
                : p{other.p}, name{p->ref(other.name)}, id{other.id} {}

            shared_resource(shared_resource && other)
                : p{other.p}, name{other.name}, id{other.id}
            {
                other.name = null_name();
            }

            shared_resource& operator = (shared_resource other)
            {
                swap(*this, other);
                return *this;
            }

            ~shared_resource()
            {
                if (p != nullptr && name != null_name())
                    p->unref(name);
            }

            // accessors
            auto get() const { return id; }
            operator auto () const { return id; }
            auto operator * () const { return id; }

        private:
            pool* p;
            name_type name;
            resource_id_type id;

            friend void swap(shared_resource& a, shared_resource& b)
            {
                std::swap(a.p, b.p);
                std::swap(a.name, b.name);
                std::swap(a.id, b.id);
            }

            shared_resource(pool& p, name_type name, resource_id_type id)
                : p{&p}, name{p.ref(name)}, id{id} {}
        };
    }
}


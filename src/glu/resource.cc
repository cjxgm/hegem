#include "resource.hh"

namespace rt::glu
{
    inline namespace resource_details
    {
        int resource_pool_count{};

        void resource_recycler::try_recycle()
        {
            for (auto& f: fs) f();
        }

        resource_recycler& resource_recycler::instance()
        {
            static resource_recycler rr;
            return rr;
        }
    }
}


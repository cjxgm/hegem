#pragma once
#include "../lib/std/string-view.hxx"
#include <string>
#include <stdexcept>

namespace rt::util
{
    inline namespace as_czstring_details
    {
        // A reference to immutable zero-terminated string.
        // It always copies. (Since it's just a reference anyway)
        struct as_czstring
        {
            using char_type = char;     // only support UTF-8
            using traits = std::char_traits<char_type>;
            using size_type = std::size_t;

            as_czstring(as_czstring const& other) = default;
            as_czstring(as_czstring     && other) = default;
            as_czstring& operator = (as_czstring const& other) = default;
            as_czstring& operator = (as_czstring     && other) = default;

            // canonical constructor
            as_czstring(char_type const* str, size_type len)
                : data_{str}, size_{len} { ensure_z(); }

            // convenient constructors
            as_czstring(std::string const& str)
                : as_czstring{str.data(), str.size()} {}

            as_czstring(char_type const* str)
                : as_czstring{str, traits::length(str)} {}

            as_czstring(lib::string_view str)
                : as_czstring{str.data(), str.size()} {}

            // accessors
            auto data() const { return data_; }
            auto size() const { return size_; }
            auto length() const { return size(); }
            auto view() const { return lib::string_view{data_, size_}; }
            auto to_string() const { return std::string{data_, size_}; }

            operator char_type const* () const { return data(); }
            operator lib::string_view () const { return view(); }
            operator std::string () const { return to_string(); }

            auto operator [] (size_type pos) const { return data_[pos]; }

        private:
            char_type const* data_;
            size_type size_;

            void ensure_z() const
            {
                if (data_[size_] != 0)
                    throw std::logic_error{"String passed to as_czstring must be zero terminated."};
            }
        };

        inline bool operator == (as_czstring const& a, as_czstring const& b) { return a.view() == b.view(); }
        inline bool operator != (as_czstring const& a, as_czstring const& b) { return a.view() != b.view(); }
        inline auto to_string(as_czstring x) { return x.to_string(); }
    }
}


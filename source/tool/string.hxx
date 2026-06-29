#pragma once
#include "types.hxx"

namespace hegem::tool
{
    struct string
    // A string is a non-owning fat pointer to some bytes.
    //
    // Fundamental Design Philosophy:
    // 1. Encourage users to use strings without index or length arithmetics at their side.
    // 2. Monotonically decreasing length after sequence of operations.
    // 2. Simple yet powerful. Tools compose and complement each other.
    // 4. Bounded toolset, as that is almost always what you wanted.
    {
        using self = string;

        char* data;
        usize length;

        /// Creating.
        static constexpr auto from(char const* data, usize length) -> string;
        static constexpr auto from_last(usize length, char const* last) -> string;
        static constexpr auto from_interval(char const* first, char const* last) -> string;
        static constexpr auto from_sentinel(char const* data) -> string;
        static constexpr auto from_owner(auto const* owner) -> string requires requires { owner->data(); owner->size(); };

        /// Bounding.
        constexpr auto bound(usize count) -> usize;  // Bound the count within this->length.
        constexpr auto monus(usize count) -> usize;  // Saturating this->length - count.

        constexpr auto prefix(usize prefix_length) -> string;   // Keep begin() intact, end() becomes begin() + bound(prefix_length)
        constexpr auto chop(usize count) -> string;             // Keep begin() intact, end() becomes end() - bound(count).
        constexpr auto suffix(usize suffix_length) -> string;   // Keep end() intact, begin() becomes end() - bound(suffix_length).
        constexpr auto skip(usize count) -> string;             // Keep end() intact, begin() becomes begin() + bound(count).

        constexpr auto starts_with(string needle) -> bool;
        constexpr auto ends_with(string needle) -> bool;

        /// Searching.
        template <char... Needles> constexpr auto find_first_any_of() -> usize;  // -> unmatching_length. this->find_first_any_of<>() always return this->length, because every byte is outside the empty set.
        template <char... Needles> constexpr auto find_first_not_in() -> usize;  // -> unmatching_length. this->find_first_not_in<>() always return 0, because the first byte is not inside the empty set.
        template <char... Needles> constexpr auto find_last_any_of() -> usize;   // -> unmatching_length. Search and count unmatching length from end() towards begin().
        template <char... Needles> constexpr auto find_last_not_in() -> usize;   // -> unmatching_length. Search and count unmatching length from end() towards begin().

        /// Copying.
        constexpr auto zero() -> string;  // -> written_part. That is, the entire this[0].
        constexpr auto fill(char source) -> string;  // -> written_part. That is, the entire this[0].
        constexpr auto ingest(string source) -> string;  // -> written_part.
        template <typename Owner> constexpr auto into_owner() -> Owner requires requires (char* data, usize length) { Owner{data, length}; };

        /// Complying iteration_protocol.
        constexpr auto begin() -> char*;
        constexpr auto end() -> char*;

        /// Complying equality_protocol.
        constexpr auto equals(string that) -> bool;
        constexpr auto operator == (string that) -> bool;
        constexpr auto operator != (string that) -> bool;

        /// Complying borrower_protocol.
        constexpr auto is(string that) -> bool;
        constexpr auto definite() -> bool;
        explicit constexpr operator bool ();
    };
}

namespace hegem::tool::literal
{
    constexpr auto operator ""_s (char const* data, usize length) -> string;
}

namespace hegem
{
    using hegem::tool::literal::operator ""_s;
}

///--- Implementation. Not design contract any more.

namespace hegem::tool
{
    inline constexpr auto string::from(char const* data, usize length) -> string
    {
        // Preserve the data pointer.
        return {(char*) data, length};
    }

    inline constexpr auto string::from_last(usize length, char const* last) -> string
    {
        return self::from(last - length, length);
    }

    inline constexpr auto string::from_interval(char const* first, char const* last) -> string
    {
        // Preserve the first pointer.
        return self::from(first, (usize) (last >= first ? last - first : 0));
    }

    inline constexpr auto string::from_sentinel(char const* data) -> string
    {
        return self::from(data, __builtin_strlen(data));
    }

    inline constexpr auto string::from_owner(auto const* owner) -> string
    requires requires { owner->data(); owner->size(); }
    {
        return self::from(owner->data(), owner->size());
    }

    inline constexpr auto string::bound(usize count) -> usize
    {
        return (count <= this->length ? count : this->length);
    }

    inline constexpr auto string::monus(usize count) -> usize
    {
        return (count <= this->length ? this->length - count : 0u);
    }

    inline constexpr auto string::prefix(usize prefix_length) -> string
    {
        return self::from(this->begin(), this->bound(prefix_length));
    }

    inline constexpr auto string::chop(usize count) -> string
    {
        return self::from(this->begin(), this->monus(count));
    }

    inline constexpr auto string::suffix(usize suffix_length) -> string
    {
        return self::from_last(this->bound(suffix_length), this->end());
    }

    inline constexpr auto string::skip(usize count) -> string
    {
        return self::from_last(this->monus(count), this->end());
    }

    inline constexpr auto string::starts_with(string needle) -> bool
    {
        return this->prefix(needle.length).equals(needle);
    }

    inline constexpr auto string::ends_with(string needle) -> bool
    {
        return this->suffix(needle.length).equals(needle);
    }

    template <char... Needles>
    inline constexpr auto string::find_first_any_of() -> usize
    {
        if constexpr (sizeof...(Needles) > 0u) {
            for (auto first = this->begin(), last = this->end(), p = first; p < last; p++) {
                if (((p[0] == Needles) || ...)) {
                    return (usize) (p - first);
                }
            }
            return this->length;
        } else {
            return this->length;  // Let's force the help on GCC anyway, now that it needs some help in find_first_not_in().
        }
    }

    template <char... Needles>
    inline constexpr auto string::find_first_not_in() -> usize
    {
        if constexpr (sizeof...(Needles) > 0u) {
            for (auto first = this->begin(), last = this->end(), p = first; p < last; p++) {
                if (((p[0] != Needles) && ...)) {
                    return (usize) (p - first);
                }
            }
            return this->length;
        } else {
            return 0;  // GCC needs some help for optimization.
        }
    }

    template <char... Needles>
    inline constexpr auto string::find_last_any_of() -> usize
    {
        if constexpr (sizeof...(Needles) > 0u) {
            for (auto first = this->begin(), last = this->end(), p = last; p > first; p--) {
                if (((p[-1] == Needles) || ...)) {
                    return (usize) (last - p);
                }
            }
            return this->length;
        } else {
            return this->length;  // Let's force the help on GCC anyway, now that it needs some help in find_last_not_in().
        }
    }

    template <char... Needles>
    inline constexpr auto string::find_last_not_in() -> usize
    {
        if constexpr (sizeof...(Needles) > 0u) {
            for (auto first = this->begin(), last = this->end(), p = last; p > first; p--) {
                if (((p[-1] != Needles) && ...)) {
                    return (usize) (last - p);
                }
            }
            return this->length;
        } else {
            return 0;  // GCC needs some help for optimization.
        }
    }

    inline constexpr auto string::zero() -> string
    {
        return this->fill('\x00');
    }

    inline constexpr auto string::fill(char source) -> string
    {
        for (auto& byte: this[0]) byte = source;
        return this[0];
    }

    inline constexpr auto string::ingest(string source) -> string
    {
        auto result = this->prefix(source.length);
        __builtin_memcpy(result.data, source.data, result.length);
        return result;
    }

    template <typename Owner>
    inline constexpr auto string::into_owner() -> Owner
    requires requires (char* data, usize length) { Owner{data, length}; }
    {
        return Owner{this->data, this->length};
    }

    inline constexpr auto string::begin() -> char*
    {
        return data;
    }

    inline constexpr auto string::end() -> char*
    {
        return data + length;
    }

    inline constexpr auto string::equals(string that) -> bool
    {
        if (!this->is(that)) {  // [[likely]]
            if (this->length != that.length) {  // [[likely]]
                return false;
            } else {
                return (__builtin_memcmp(this->data, that.data, this->length) == 0);
            }
        } else {
            return true;
        }
    }

    inline constexpr auto string::operator == (string that) -> bool { return this->equals(that); }
    inline constexpr auto string::operator != (string that) -> bool { return !this->equals(that); }

    inline constexpr auto string::is(string that) -> bool
    {
        return (this->data == that.data && this->length == that.length);
    }

    inline constexpr auto string::definite() -> bool
    {
        return (this->length > 0u);
    }

    inline constexpr string::operator bool () { return this->definite(); }
}

namespace hegem::tool::literal
{
    inline constexpr auto operator ""_s (char const* data, usize length) -> string
    {
        return string::from((char*) data, length);
    }
}


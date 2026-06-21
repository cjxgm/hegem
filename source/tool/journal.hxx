#pragma once
#include "as-czstring.hxx"

namespace hegem::tool
{
    inline namespace journal_details
    {
        struct journal
        {
            journal(char const* component);
        };

        journal operator << (journal j, as_czstring x);
        journal operator << (journal j, void const* x);
        journal operator << (journal j, float x);
        journal operator << (journal j, int x);

        template <class T>
        inline journal operator << (journal j, T const* p) { return (j << static_cast<void const*>(p)); }
        inline journal operator << (journal j, char const* p) { return (j << as_czstring{p}); }
    }
}


#pragma once
#include <string>

namespace rt::utils
{
    inline namespace journal_details
    {
        struct journal
        {
            journal(char const* component);
        };

        journal operator << (journal j, void const* str);
        journal operator << (journal j, char const* str);

        journal operator << (journal j, int x);
        journal operator << (journal j, float x);

        template <class T>
        inline journal operator << (journal j, T const* p) { return (j << static_cast<void const*>(p)); }
        inline journal operator << (journal j, std::string const& str) { return (j << str.data()); }
    }
}


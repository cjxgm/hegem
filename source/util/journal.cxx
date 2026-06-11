#include "journal.hxx"
#include <iostream>

namespace hegem::util
{
    inline namespace journal_details
    {
        namespace
        {
            template <class T>
            journal write(journal j, T&& x)
            {
                std::cerr << x;
                return j;
            }
        }

        journal::journal(char const* component)
        {
            std::cerr << "\e[1;34m[" << component << "]\e[0m ";
        }

        journal operator << (journal j, as_czstring str) { return write(j, str); }
        journal operator << (journal j, void const* p) { return write(j, p); }
        journal operator << (journal j, float x) { return write(j, x); }
        journal operator << (journal j, int x) { return write(j, x); }
    }
}


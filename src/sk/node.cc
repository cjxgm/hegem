#include "node.hh"

namespace rt::sk
{
    namespace
    {
        // Returns true if [a0, a1] ∩  [b0, b1] != ∅
        bool overlaps(int a0, int a1, int b0, int b1)
        {
            return (a0 <= b1) && (b0 <= a1);
        }

        bool xoverlaps(node const& a, node const& b)
        {
            return overlaps(a.x, a.x+a.width-1, b.x, b.x+b.width-1);
        }
    }

    bool contains(node const& n, int x, int y)
    {
        return (y == n.y) && overlaps(n.x, n.x+n.width-1, x, x);
    }

    bool overlaps(node const& a, node const& b)
    {
        return (a.y == b.y) && xoverlaps(a, b);
    }

    bool connects(node const& above, node const& below)
    {
        return (above.y+1 == below.y) && xoverlaps(above, below);
    }
}


#include <iostream>
#include "dump.hh"
#include "list.hh"
#include "primitive.hh"

namespace rt::hemesh
{
    void dump(rt::hemesh::body_type* body)
    {
        for (auto& b: list::iterate(body)) {
            std::cerr << "# body " << &b << "\n";
            for (auto& f: list::iterate(b.any_face)) {
                std::cerr << "= face " << &f << "\n";
                for (auto& r: list::iterate(f.boundary)) {
                    std::cerr << "* ring " << &r << " hege " << r.any_hege << "\n";
                    for (auto& h: list::iterate(r.any_hege)) {
                        auto& p = h.start->pos;
                        std::cerr << "  - hege " << &h << " " << p.x << "," << p.y << "," << p.z << "\n";
                        std::cerr << "    twin " << h.twin << " around " << h.start->any_hege << "\n";
                    }
                }
            }
        }
    }
}


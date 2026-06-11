#pragma once
#include "../util/slab.hxx"
#include "hemesh.fwd.hxx"
#include "primitive.hxx"

namespace hegem::swing
{
    struct hemesh
    {
        hemesh() = default;
        hemesh(hemesh &&) = default;
        hemesh& operator = (hemesh &&) = default;

        hemesh(hemesh const& m)
        {
            extend(m);
        }

        hemesh& operator = (hemesh const& m)
        {
            using std::swap;
            hemesh x{m};
            swap(*this, x);
            return *this;
        }

    public: // misc
        void extend(hemesh const& m);
        void diagnose(char const* situation="at random location") const;

    public: // makers establishes internal data structures.
        body_type* make_body();
        face_type* make_face(body_type* body);
        vert_type* make_vert();
        ring_type* make_ring(face_type* face, vert_type* vert);
        hege_type* make_hege_twin(ring_type* ring, vert_type* vert);
        hege_type* make_hege_twin(hege_type* hege, vert_type* vert);
        edge_type* make_edge(hege_type* hege);
        void close_hege(hege_type* h0, hege_type* h1);

    public: // free(T*)
        #define STRUCT(NAME, VAR) void free(NAME* VAR) { VAR ## s.free(VAR); }
        #include "primitive.inl"

    public: // data structure
        body_type* any_body{};

        // Expands to `unbound_slab<body_type> bodys;` and etc.
        #define STRUCT(NAME, VAR) util::unbound_trivial_slab<NAME> VAR ## s;
        #include "primitive.inl"
    };
}


#pragma once
#include "../lib/glm/vec3.hh"

namespace rt::hemesh
{
    inline namespace primitive
    {
        using position_type = glm::vec3;

        struct body_type;
        struct face_type;
        struct ring_type;
        struct edge_type;
        struct hege_type;
        struct vert_type;

        struct body_type
        {
            body_type* next;
            body_type* prev;
            face_type* first_face;
        };

        struct face_type
        {
            face_type* next;
            face_type* prev;
            body_type* body;
            ring_type* boundary;
        };

        // A "ring" represent a loop. (Yeah, yeah, I know).
        //
        // First ring (face->boundary) in the ring list is the boundary loop.
        // Other rings are inner loops.
        //
        // Inner and boundary loops must not intersect each other.
        // Inner loops must not contains any other loop.
        struct ring_type
        {
            ring_type* next;
            ring_type* prev;
            face_type* face;
            vert_type* first_vert;
            hege_type* first_hege;
        };

        struct edge_type
        {
            hege_type* first_hege;
        };

        struct hege_type
        {
            hege_type* next;
            hege_type* prev;
            hege_type* twin;
            ring_type* ring;
            vert_type* start;
            edge_type* edge;
        };

        struct vert_type
        {
            hege_type* first_hege;
            position_type pos;
        };
    }
}


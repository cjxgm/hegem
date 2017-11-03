//#pragma once      // No #pragma once intentionally.
// Primitive data structures description
#ifndef STRUCT
    #define STRUCT(NAME)
#endif

#ifndef END_STRUCT
    #define END_STRUCT(VAR)
#endif

#ifndef FIELD_PTR_FROM_SLAB   // TYPE* is a pointer allocated from a slab
    #define FIELD_PTR_FROM_SLAB(TYPE, VAR)
#endif

#ifndef FIELD
    #define FIELD(TYPE, VAR)
#endif

STRUCT(body_type)
    FIELD_PTR_FROM_SLAB(body_type, next)
    FIELD_PTR_FROM_SLAB(body_type, prev)
    FIELD_PTR_FROM_SLAB(face_type, any_face)
END_STRUCT(body)

STRUCT(face_type)
    FIELD_PTR_FROM_SLAB(face_type, next)
    FIELD_PTR_FROM_SLAB(face_type, prev)
    FIELD_PTR_FROM_SLAB(body_type, body)
    FIELD_PTR_FROM_SLAB(ring_type, boundary)
END_STRUCT(face)

// A "ring" represent a loop. (Yeah, yeah, I know).
//
// First ring (face->boundary) in the ring list is the boundary loop.
// Other rings are inner loops.
//
// Inner and boundary loops must not intersect each other.
// Inner loops must not contains any other loop.
STRUCT(ring_type)
    FIELD_PTR_FROM_SLAB(ring_type, next)
    FIELD_PTR_FROM_SLAB(ring_type, prev)
    FIELD_PTR_FROM_SLAB(face_type, face)
    FIELD_PTR_FROM_SLAB(vert_type, any_vert)
    FIELD_PTR_FROM_SLAB(hege_type, any_hege)
END_STRUCT(ring)

STRUCT(edge_type)
    FIELD_PTR_FROM_SLAB(hege_type, any_hege)
END_STRUCT(edge)

// hege = Half EdGE
STRUCT(hege_type)
    FIELD_PTR_FROM_SLAB(hege_type, next)
    FIELD_PTR_FROM_SLAB(hege_type, prev)
    FIELD_PTR_FROM_SLAB(hege_type, twin)
    FIELD_PTR_FROM_SLAB(ring_type, ring)
    FIELD_PTR_FROM_SLAB(vert_type, start)
    FIELD_PTR_FROM_SLAB(edge_type, edge)
END_STRUCT(hege)

STRUCT(vert_type)
    FIELD_PTR_FROM_SLAB(hege_type, any_hege)
    FIELD(position_type, pos)
END_STRUCT(vert)

#undef STRUCT
#undef END_STRUCT
#undef FIELD_PTR_FROM_SLAB
#undef FIELD


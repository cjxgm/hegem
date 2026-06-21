//#pragma once      // No #pragma once intentionally.
// Primitive data structures description

//---- You should define (one of or many of) the following macros.
// structure definition
#ifndef STRUCT
    #define STRUCT(NAME, VAR)
#endif

#ifndef END_STRUCT
    #define END_STRUCT()
#endif

#ifndef FIELD_PTR_FROM_SLAB   // TYPE* is a pointer allocated from a slab
    #define FIELD_PTR_FROM_SLAB(TYPE, VAR)
#endif

#ifndef FIELD
    #define FIELD(TYPE, VAR)
#endif

// relationships
#ifndef CHILD_PARENT_RELATION
    #define CHILD_PARENT_RELATION(CHILD, PARENT)
#endif

#ifndef CHILDREN_PARENT_RELATION
    #define CHILDREN_PARENT_RELATION(CHILD_FIRST, PARENT) \
        CHILDREN_PARENT_RELATION_CUSTOM_NEXT(CHILD_FIRST, PARENT, next)      // fallback
#endif

#ifndef CHILDREN_PARENT_RELATION_CUSTOM_NEXT
    #define CHILDREN_PARENT_RELATION_CUSTOM_NEXT(CHILD_FIRST, PARENT, NEXT) \
        CHILD_PARENT_RELATION(CHILD_FIRST, PARENT)      // fallback
#endif

//---- You'd better NOT define any of the following macros.
#ifndef LINKABLE_STRUCT
    #define LINKABLE_STRUCT(NAME, VAR) \
        STRUCT(NAME, VAR) \
            FIELD_PTR_FROM_SLAB(NAME, next) \
            FIELD_PTR_FROM_SLAB(NAME, prev) \
            CHILD_PARENT_RELATION(next, prev)
#endif

//---- Data structure description
LINKABLE_STRUCT(body_type, body)
    FIELD_PTR_FROM_SLAB(face_type, any_face)
    CHILDREN_PARENT_RELATION(any_face, body)
END_STRUCT()

LINKABLE_STRUCT(face_type, face)
    FIELD_PTR_FROM_SLAB(body_type, body)
    FIELD_PTR_FROM_SLAB(ring_type, boundary)
    CHILDREN_PARENT_RELATION(boundary, face)
END_STRUCT()

// A "ring" represent a loop. (Yeah, yeah, I know).
//
// First ring (face->boundary) in the ring list is the boundary loop.
// Other rings are inner loops.
//
// Inner and boundary loops must not intersect each other.
LINKABLE_STRUCT(ring_type, ring)
    FIELD_PTR_FROM_SLAB(face_type, face)
    FIELD_PTR_FROM_SLAB(vert_type, any_vert)
    FIELD_PTR_FROM_SLAB(hege_type, any_hege)
    CHILDREN_PARENT_RELATION(any_hege, ring)
END_STRUCT()

STRUCT(edge_type, edge)
    FIELD_PTR_FROM_SLAB(hege_type, any_hege)
    CHILD_PARENT_RELATION(any_hege, edge)
    CHILD_PARENT_RELATION(any_hege->twin, edge)
END_STRUCT()

// hege = Half EdGE
LINKABLE_STRUCT(hege_type, hege)
    FIELD_PTR_FROM_SLAB(hege_type, twin)
    FIELD_PTR_FROM_SLAB(ring_type, ring)
    FIELD_PTR_FROM_SLAB(vert_type, start)
    FIELD_PTR_FROM_SLAB(edge_type, edge)
END_STRUCT()

STRUCT(vert_type, vert)
    FIELD_PTR_FROM_SLAB(hege_type, any_hege)
    FIELD(position_type, pos)
    CHILDREN_PARENT_RELATION_CUSTOM_NEXT(any_hege, start, prev->twin)
END_STRUCT()

//---- ALL MACROS SHOULD BE UNDEF-ED
#undef STRUCT
#undef END_STRUCT
#undef FIELD_PTR_FROM_SLAB
#undef FIELD
#undef CHILD_PARENT_RELATION
#undef CHILDREN_PARENT_RELATION
#undef CHILDREN_PARENT_RELATION_CUSTOM_NEXT
#undef LINKABLE_STRUCT


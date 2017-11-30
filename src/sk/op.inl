// #pragma once         // no #pragma once intentionally

#ifndef KIND
    #define KIND(ID, NAME, TOOLTIP)
#endif

#ifndef OP
    // ARITY: 0 for no input; N for if and only if N inputs. -1 for infinite amount of inputs.
    #define OP(KIND, ID, ARITY, NAME, TOOLTIP, FIELDS...)
#endif

#ifndef FIELD
    // ARITY: 0 for no input; N for if and only if N inputs. -1 for infinite amount of inputs.
    #define FIELD(TYPE, VAR, INITIAL, EDITING_WIDGET, NAME, TOOLTIP)
#endif

KIND(system, "System", "Core operators.")
KIND(primitive, "Primitive", "Basic shapes.")
KIND(advanced, "Advanced", "Advanced modeling operations.")

OP(system, nop, 1, "Nop", "No operation. This is used to fill gaps between nodes.")

OP(primitive, poly_disk, 0, "Poly Disk", "Polygonal disk centered at origin and facing y+ axis.",
    FIELD(float, radius, 1.0f, number, "Radius", "")
    FIELD(int, ngon, 6, number, "N-gon", "Number of vertices")
)

OP(primitive, cube, 0, "Cube", "A cube with edge length of 1.")

OP(advanced, merge, -1, "Merge", "Combine 2 models into 1 model with extra bodies. This is NOT a gluing operation.")

#undef KIND
#undef OP
#undef FIELD


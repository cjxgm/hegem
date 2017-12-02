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
KIND(selection, "Selection", "(De)select vertices and faces as operation targets.")
KIND(transform, "Transform", "Affine transformations.")
KIND(advanced, "Advanced", "Advanced modeling operations.")

OP(system, nop, 1, "Nop", "No operation. This is used to fill gaps between nodes.")

OP(primitive, poly_disk, 0, "Poly Disk", "Polygonal disk centered at origin and facing y+ axis.",
    FIELD(float, radius, 1.0f, value, "Radius", "")
    FIELD(int, ngon, 6, value, "N-gon", "Number of vertices")
)

OP(primitive, cube, 0, "Cube", "A cube with edge length of 1.")

OP(selection, select_verts, 1, "Select Verts", "(De)select vertices lied in a sphere.",
    FIELD(bool, inverse, false, value, "Inverse",
        "When off, select the matching vertices.\n"
        "When on, select the non-matching vertices."
    )
    FIELD(bool, exclusive, true, value, "Exclusive",
        "When off, merge the current selection with the previous selection.\n"
        "When on, replace previsous selection with current selection."
    )
    FIELD(float3, center, {}, value, "Center", "Center of the matching sphere.")
    FIELD(float, radius, 0.1f, value, "Radius",
        "Radius of the matching sphere.\n"
        "Nothing will match if negative."
    )
)

OP(selection, select_faces, 1, "Select Faces", "(De)select faces lied in a sphere.",
    FIELD(bool, inverse, false, value, "Inverse",
        "When off, select the matching faces.\n"
        "When on, select the non-matching faces."
    )
    FIELD(bool, exclusive, true, value, "Exclusive",
        "When off, merge the current selection with the previous selection.\n"
        "When on, replace previsous selection with current selection."
    )
    FIELD(bool, affect_verts, true, value, "Also select vertices",
        "When on, also select the vertices of the matching faces\n"
        "with the same inverseness and exclusiveness as in this node."
    )
    FIELD(float3, center, {}, value, "Center", "Center of the matching sphere.")
    FIELD(float, radius, 0.1f, value, "Radius",
        "Radius of the matching sphere.\n"
        "Nothing will match if negative."
    )
    FIELD(bool, no_backfaces, true, value, "No backfaces", "When on, treat all backfaces as non-matching faces.")
    FIELD(float3, front, float3({0.0f, 1.0f, 0.0f}), value, "Front", "Define the front direction.")
)

OP(transform, move, 1, "Move", "Move selected vertices.",
    FIELD(float3, offset, {}, value, "Offset", "How much to move.")
    FIELD(bool, use_median, false, value, "Use median point as offset", "Use the barycenter/averaged point as the offset.")
    FIELD(bool, reverse, false, value, "Reverse direction", "Negate the offset.")
)

OP(transform, scale, 1, "Scale", "Scale selected vertices.",
    FIELD(float3, amount, float3({1.0f, 1.0f, 1.0f}), value, "Amount", "Scaling amount per axis.")
    FIELD(float, uniform_amount, 1.0f, value, "Uniform", "Uniform scaling amount.")
    FIELD(float3, pivot, {}, value, "Pivot", "Scaling around this point.")
    FIELD(bool, use_median, true, value, "Use median point as pivot", "Use the barycenter/averaged point as the pivot.")
)

OP(transform, rotate, 1, "Rotate", "Rotate selected vertices.",
    FIELD(float3, axis, float3({0.0f, 1.0f, 0.0f}), value, "Axis", "Rotation axis.")
    FIELD(float, amount, {}, value, "Degrees", "How much to rotate around the rotation axis.")
    FIELD(float3, pivot, {}, value, "Pivot", "Rotating around this point.")
    FIELD(bool, use_median, true, value, "Use median point as pivot", "Use the barycenter/averaged point as the pivot.")
)

OP(advanced, merge, -1, "Merge", "Combine 2 models into 1 model with extra bodies.\nThis is NOT a gluing operation.")

OP(advanced, extrude, 1, "Extrude", "Extrude a face along its normal positively.",
    FIELD(float, amount, 1.0f, value, "Amount", "How much to extrude.")
    FIELD(bool, select_verts, true, value, "Select new vertices", "Exclusively select the new vertices added by the extrusion")
)

#undef KIND
#undef OP
#undef FIELD


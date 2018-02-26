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

KIND(indirection, "Indirection",
    "All problems in computer science can be solved by another level of indirection.\n"
    "--- David J. Wheeler"
)
KIND(primitive, "Primitive", "Basic shapes.")
KIND(selection, "Selection", "(De)select vertices and faces as operation targets.")
KIND(transform, "Transform", "Affine transformations.")
KIND(repetition, "Repetition", "Repeatedly clone bodies.")
KIND(advanced, "Advanced", "Advanced modeling operations.")
KIND(subdivision, "Subdivision", "Subdivision surface operations.")

OP(indirection, nop, 1, "Nop", "No operation but passthrough the input to output.")

OP(indirection, pack, -1, "Pack",
    "Merge multiple inputs into one single pack.\n"
    "Each input can be extracted by Unpack in desired order."
)

OP(indirection, unpack, 1, "Unpack", "Extract a value out of a pack.",
    FIELD(bool, exclusive, true, value, "Exclusive",
        "When off, copy the value out of the pack.\n"
        "When on, move the value out of the pack, making later extraction at the same index fail."
    )
    FIELD(int, index, 0, value, "Index", "Which value to extract")
)

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

OP(selection, select_faces, 1, "Select Faces", "(De)select faces whose barycenters lie in a sphere.",
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

OP(selection, containing_faces, 1, "Containing Faces", "(De)select faces that contains the selected vertices.",
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
    FIELD(bool, only_boundary, true, value, "Only boundary", "When on, ignore all vertices inside faces.")
    FIELD(bool, no_backfaces, true, value, "No backfaces", "When on, treat all backfaces as non-matching faces.")
    FIELD(float3, front, float3({0.0f, 1.0f, 0.0f}), value, "Front", "Define the front direction.")
)

OP(transform, move, 1, "Move", "Move selected vertices.",
    FIELD(float3, offset, {}, value, "Offset", "How much to move.")
    FIELD(bool, use_median, false, value, "Use median point as offset", "Use the middle point as the offset.")
    FIELD(bool, reverse, false, value, "Reverse direction", "Negate the offset.")
)

OP(transform, scale, 1, "Scale", "Scale selected vertices.",
    FIELD(float3, amount, float3({1.0f, 1.0f, 1.0f}), value, "Amount", "Scaling amount per axis.")
    FIELD(float, uniform_amount, 1.0f, value, "Uniform", "Uniform scaling amount.")
    FIELD(float3, pivot, {}, value, "Pivot", "Scaling around this point.")
    FIELD(bool, use_median, false, value, "Use median point as pivot", "Use the middle point as the pivot.")
)

OP(transform, rotate, 1, "Rotate", "Rotate selected vertices.",
    FIELD(float3, axis, float3({0.0f, 1.0f, 0.0f}), value, "Axis", "Rotation axis.")
    FIELD(float, amount, {}, value, "Degrees", "How much to rotate around the rotation axis.")
    FIELD(float3, pivot, {}, value, "Pivot", "Rotating around this point.")
    FIELD(bool, use_median, false, value, "Use median point as pivot", "Use the middle point as the pivot.")
)

OP(repetition, array, 1, "Array", "Repeatedly clone the whole model along a straight line.",
    FIELD(float3, offset, float3({2.0f, 0.0f, 0.0f}), value, "Offset", "Offset of each clone.")
    FIELD(int, count, 3, value, "Count", "How many to clone.")
    FIELD(bool, recenter, false, value, "Re-center", "Move the cloned models from their pivots to the origin.")
)

OP(repetition, spin, 1, "Spin",
    "Repeatedly clone the whole model along a circle centered at origin pointing at y+ axis.",
    FIELD(bool, local_rotation, false, value, "Local rotation", "Rotate each clone locally.")
    FIELD(float, start, 0.0f, value, "Start", "Starting angle.")
    FIELD(float, radius, 1.0f, value, "Radius", "Radius of the circle.")
    FIELD(float, fraction, 1.0f, value, "Fraction", "Fraction of the circle.")
    FIELD(bool, exclusive_end, true, value, "Exclusive end", "Exclude the arc end.")
    FIELD(int, count, 3, value, "Count", "How many clones to make.")
)

OP(advanced, merge, -1, "Merge", "Combine 2 models into 1 model with extra bodies.\nThis is NOT a gluing operation.")

OP(advanced, extrude, 1, "Extrude", "Extrude a face along its normal positively.\nYou may extrude inward with negative amount, but it will NOT produce holes.",
    FIELD(float, amount, 1.0f, value, "Amount", "How much to extrude.")
    FIELD(bool, select_verts, true, value, "Select new vertices", "Exclusively select the new vertices added by the extrusion")
)

OP(advanced, embed, 2, "Embed",
    "Project and glue double-sided faces onto a face of another model.\n"
    "Each body must be a single double-sided face.\n"
    "The double-sided faces cannot contain inner rings.\n"
    "Do not support gluing solid models.",
    FIELD(bool, no_holes, false, value, "No holes", "Do not attempt to punch holes even if applicable.")
)

OP(subdivision, catmull_clark, 1, "Catmull Clark", "Catmull Clark subdivision surface.",
    FIELD(bool, triangulate, false, value, "Triangulate", "Triangulate before subdividing.\nRemove holes and concaves to work around subdivision limitations.")
    FIELD(int, level, 3, value, "Level", "Subdivision level.")
)

#undef KIND
#undef OP
#undef FIELD


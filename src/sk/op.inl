// #pragma once         // no #pragma once intentionally

#ifndef KIND
    #define KIND(ID, NAME, TOOLTIP)
#endif

#ifndef SECTION
    #define SECTION(ID, NAME, TOOLTIP)
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
KIND(timeline_primitive, "Primitive", "Basic timelines.")
KIND(timeline_timewarping, "Timewarping", "Changes how time flows.")
KIND(timeline_tweening, "Tweening", "Non-linear interpolation.")
KIND(timeline_arithmetic, "Arithmetic", "Basic math operators.")
KIND(timeline_coordinate, "Coordinate", "Convert between coordinate systems.")
KIND(timeline_system, "System", "Use the curves/waves to drive something.")

SECTION(generic, "Generic", "Can be used anywhere.")

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

SECTION(modeling, "Modeling", "Create models.")

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

SECTION(timeline, "Timeline", "Create curves and waves to drive particle systems and sound systems.")

OP(timeline_primitive, constant, 0, "Constant", "A line parallel to the x-axis.",
    FIELD(float, value, 0.0f, value, "Value", "Value of the constant.")
)
OP(timeline_primitive, ramp, 0, "Ramp", "An increasing line.")
OP(timeline_primitive, id, 0, "Identity", "A number that is unique to each instance of the timeline.")
OP(timeline_primitive, apply, 2, "Apply", "Apply a timeline as if it was a function.",
    FIELD(float, value_min, 0.0f, value, "Value Min", "Minimum value of the resulting timeline.")
    FIELD(float, value_max, 1.0f, value, "Value Max", "Maximum value of the resulting timeline.")
    FIELD(float, window_min, 0.0f, value, "Window Min", "Minimum window of the resulting timeline.")
    FIELD(float, window_max, 1.0f, value, "Window Max", "Maximum window of the resulting timeline.")
)

OP(timeline_timewarping, timeshift, 1, "Timeshift", "Skip forward to a future time or backward to a history time.",
    FIELD(float, amount, 1.0f, value, "Amount", "How much time to skip.\nPositive number to skip forward.\nNegative number to skip backward.")
)
OP(timeline_timewarping, speedup, 1, "Speedup", "Make the time flows faster or slower.",
    FIELD(float, scale, 2.0f, value, "Scale", "How to flow the time.\n`scale > 1` to flow faster.\n`0 < scale < 1` to flow slower.\n`scale < 0` to invert the time flowing direction.")
)
OP(timeline_timewarping, window, 1, "Window", "Make the time stuck in a loop.",
    FIELD(float, size, 1.0f, value, "Size", "The size of the time window.")
)

OP(timeline_tweening, easeout, 1, "Ease-out", "Slower and slower.",
    FIELD(float, target, 1.0f, value, "Target Value", "Ease from 0 to the target value.")
    FIELD(float, steepness, 10.0f, value, "Steepness", "How fast to go from 0 to the target value.")
)

OP(timeline_arithmetic, add, 2, "Add", "Addition.")
OP(timeline_arithmetic, sub, 2, "Sub", "Subtraction.")
OP(timeline_arithmetic, mul, 2, "Mul", "Multiplication.")
OP(timeline_arithmetic, div, 2, "Div", "Division.")
OP(timeline_arithmetic, mod, 2, "Mod", "Modulus. The result is guaranteed to be positive.")

OP(timeline_coordinate, polar_to_cartesian_x, 2, "Polar -> X", "Convert polar coordinate (angle, distance) to cartesian coordinate (x, y), and take the X value.")
OP(timeline_coordinate, polar_to_cartesian_y, 2, "Polar -> Y", "Convert polar coordinate (angle, distance) to cartesian coordinate (x, y), and take the Y value.")

OP(timeline_system, inspect, -1, "Inspect", "Show the input expressions in error notifications.")
OP(timeline_system, spark, 8, "Spark", "A particle system of light-emitting spheres.\nInput: x, y, z, radius, r, g, b, opacity.",
    FIELD(int, num_particles, 100, value, "Particle Amount", "How many particles should there be.")
)

#undef KIND
#undef SECTION
#undef OP
#undef FIELD


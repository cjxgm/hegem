#version 450 core

layout(quads, equal_spacing, ccw) in;

layout(location=0) uniform mat4 proj_view;
layout(location=1) uniform mat4 model;

in ctrl_eval
{
    vec3 pos;
}
prev[];

out eval_frag
{
    vec3 pos;
    vec3 normal;
}
next;

void main()
{
    vec3 p1 = mix(prev[0].pos, prev[1].pos, gl_TessCoord.x);
    vec3 p2 = mix(prev[2].pos, prev[3].pos, gl_TessCoord.x);
    vec3 p = mix(p1, p2, gl_TessCoord.y);

    vec3 model_pos = normalize(p);  // model position is also the normal vector
    vec4 world_pos = model * vec4(model_pos, 1.0f);
    vec4 clip_pos = proj_view * world_pos;

    next.pos = world_pos.xyz;
    next.normal = (vec4(model_pos, 0.0f) * inverse(model)).xyz;
    gl_Position = clip_pos;
}


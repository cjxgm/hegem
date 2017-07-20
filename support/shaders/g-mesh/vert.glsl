#version 450 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;

layout(location=0) uniform mat4 proj_view;
layout(location=1) uniform mat4 model;

out vert_frag
{
    vec3 pos;
    vec3 normal;
}
next;

void main()
{
    vec4 world_pos = model * vec4(pos, 1.0f);
    vec4 clip_pos = proj_view * world_pos;
    next.pos = world_pos.xyz;
    next.normal = (vec4(normal, 0.0f) * inverse(model)).xyz;
    gl_Position = clip_pos;
}


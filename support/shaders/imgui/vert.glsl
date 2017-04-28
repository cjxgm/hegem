#version 450 core

layout(location=0) in vec2 pos;
layout(location=1) in vec2 uv;
layout(location=2) in vec4 color;

out vert_frag
{
    vec2 uv;
    vec4 color;
}
next;

layout(location=0) uniform mat4 proj;

void main()
{
    next.uv = uv;
    next.color = color;
    gl_Position = proj * vec4(pos, 0, 1);
}


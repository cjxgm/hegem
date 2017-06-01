#version 450 core

layout(location=0) in vec3 pos;
out vert_ctrl
{
    vec3 pos;
}
next;

void main()
{
    next.pos = pos;
}


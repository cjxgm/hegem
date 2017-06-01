#version 450 core

layout(vertices=4) out;

in vert_ctrl
{
    vec3 pos;
}
prev[];

out ctrl_eval
{
    vec3 pos;
}
next[];

void main()
{
    next[gl_InvocationID].pos = prev[gl_InvocationID].pos;

    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = 6;
        gl_TessLevelInner[1] = 6;
        gl_TessLevelOuter[0] = 6;
        gl_TessLevelOuter[1] = 6;
        gl_TessLevelOuter[2] = 6;
        gl_TessLevelOuter[3] = 6;
    }
}


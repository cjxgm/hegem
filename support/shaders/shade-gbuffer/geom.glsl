#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices=12) out;

const float size = 0.1;
const vec2 verts[4][3] = {
    { vec2(-1, -1), vec2(-1+size, -1), vec2(-1, -1+size) },
    { vec2(+1, -1), vec2(+1-size, -1), vec2(+1, -1+size) },
    { vec2(+1, +1), vec2(+1-size, +1), vec2(+1, +1-size) },
    { vec2(-1, +1), vec2(-1+size, +1), vec2(-1, +1-size) },
};

void main()
{
    for (int p=0; p<4; p++) {
        for (int v=0; v<3; v++) {
            gl_Position = vec4(verts[p][v], 0, 1);
            EmitVertex();
        }
        EndPrimitive();
    }
}


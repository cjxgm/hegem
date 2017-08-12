#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices=24) out;

layout(location=0) uniform mat4 proj_view;
layout(location=1) uniform mat4 model;

in vert_geom
{
    vec4 color;
}
prev[];

out geom_frag
{
    vec4 color;
    vec3 normal;
    vec3 pos;
    vec2 uv;
}
next;

const float h = 0.5f;

const vec3 ps[8] = {
    vec3(-h, -h, +h),
    vec3(-h, +h, +h),
    vec3(+h, -h, +h),
    vec3(+h, +h, +h),
    vec3(-h, -h, -h),
    vec3(-h, +h, -h),
    vec3(+h, -h, -h),
    vec3(+h, +h, -h),
};

const int fs[6][4] = {
    { 0, 1, 2, 3 },
    { 2, 3, 6, 7 },
    { 6, 7, 4, 5 },
    { 4, 5, 0, 1 },
    { 1, 5, 3, 7 },
    { 2, 6, 0, 4 },
};

const vec3 ns[6] = {
    vec3( 0,  0, +1),
    vec3(+1,  0,  0),
    vec3( 0,  0, -1),
    vec3(-1,  0,  0),
    vec3( 0, +1,  0),
    vec3( 0, -1,  0),
};

const vec2 uvs[4] = {
    vec2(0, 0),
    vec2(0, 1),
    vec2(1, 0),
    vec2(1, 1),
};

void main()
{
    next.color = prev[0].color;
    for (int face=0; face<6; face++) {
        next.normal = ns[face];
        for (int vert=0; vert<4; vert++) {
            vec3 p = ps[fs[face][vert]];
            vec4 world_pos = model * vec4(p, 1.0f);
            gl_Position = proj_view * world_pos;
            next.pos = world_pos.xyz;
            next.uv = uvs[vert];
            EmitVertex();
        }
        EndPrimitive();
    }
}


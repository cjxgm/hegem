#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices=3) out;

layout(location=1) uniform mat4 proj_view;
out geom_frag
{
    vec4 near_point;
    vec4 far_point;
}
next;

const vec2 ps[3] = {
    vec2(-1, -1),
    vec2( 3, -1),
    vec2(-1,  3),
};

void main()
{
    mat4 ipv = inverse(proj_view);
    for (int i=0; i<3; i++) {
        vec2 p = ps[i];
        gl_Position = vec4(p, 0, 1);
        next.near_point = ipv * vec4(p, 0, 1);
        next.far_point  = ipv * vec4(p, 1, 1);
        EmitVertex();
    }
    EndPrimitive();
}


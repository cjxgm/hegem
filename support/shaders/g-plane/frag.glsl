#version 450 core

layout(location=0) uniform mat4 proj_view;
layout(location=1) uniform vec3 cam_apex;
layout(location=2) uniform vec3 pnormal;
layout(location=3) uniform float poffset;
layout(location=4) uniform vec3 albedo_color;

layout(location=0) out vec3 albedo;
layout(location=1) out vec3 normal;
layout(location=2) out vec3 position;
layout(location=3) out int material;

in geom_frag
{
    vec4 near_point;
    vec4 far_point;
}
prev;

float remap(float x, float xf, float xt, float df, float dt)
{
    return (x - xf) / (xt - xf) * (dt - df) + df;
}

void main()
{
    vec4 np = prev.near_point;
    vec4 fp = prev.far_point;
    vec3 dir = normalize(fp.xyz / fp.w - np.xyz / np.w);

    vec3 p = cam_apex - pnormal * poffset;
    float pn = dot(pnormal, p);
    float dn = dot(pnormal, dir);

    if (pn * dn < 0) {
        float extent = pn / -dn;
        vec3 pos = cam_apex + dir * extent;

        vec4 cpos = proj_view * vec4(pos, 1.0f);
        float depth = remap(cpos.z / cpos.w, -1.0f, 1.0f, 0.0f, 1.0f);

        albedo = albedo_color;
        normal = pnormal;
        position = pos;
        material = 1;
        gl_FragDepth = depth;
    } else {
        discard;
    }
}


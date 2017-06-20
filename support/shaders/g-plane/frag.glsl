#version 450 core

layout(location=0) uniform mat4 proj_view;
layout(location=1) uniform vec3 cam_apex;
layout(location=2) uniform vec3 pnormal;
layout(location=3) uniform float poffset;
layout(location=4) uniform vec3 albedo_color;
layout(location=5) uniform vec3 refl_color;
layout(location=6) uniform float roughness;
layout(location=7) uniform float ior;

layout(location=0) out vec4 albedo;
layout(location=1) out vec4 reflection;
layout(location=2) out vec4 normal;
layout(location=3) out vec4 position;

in geom_frag
{
    vec4 near_point;
    vec4 far_point;
}
prev;

const float pbr_material = 1.0f;

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

        albedo = vec4(albedo_color, roughness);
        reflection = vec4(refl_color, ior);
        normal = vec4(pnormal, 0.0f);
        position = vec4(pos, pbr_material);
        gl_FragDepth = depth;
    } else {
        discard;
    }
}


#version 450 core

layout(location=0) uniform vec3 sky_color;

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

void main()
{
    vec4 np = prev.near_point;
    vec4 fp = prev.far_point;
    vec3 dir = normalize(fp.xyz / fp.w - np.xyz / np.w);

    albedo = sky_color;
    normal = dir;
    position = vec3(0, 0, 0);
    material = 2;
}


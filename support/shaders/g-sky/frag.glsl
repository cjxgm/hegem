#version 450 core

layout(location=0) uniform vec3 sky_color;

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

const float sky_material = 2.0f;

void main()
{
    vec4 np = prev.near_point;
    vec4 fp = prev.far_point;
    vec3 dir = normalize(fp.xyz / fp.w - np.xyz / np.w);

    albedo = vec4(sky_color, 0.0f);
    reflection = vec4(0.0f);
    normal = vec4(dir, 0.0f);
    position = vec4(vec3(0.0f), sky_material);
}


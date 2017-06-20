#version 450 core

layout(location=2) uniform vec3 albedo_color;
layout(location=3) uniform vec3 refl_color;
layout(location=4) uniform float roughness;
layout(location=5) uniform float ior;

layout(location=0) out vec4 albedo;
layout(location=1) out vec4 reflection;
layout(location=2) out vec4 normal;
layout(location=3) out vec4 position;

in eval_frag
{
    vec3 pos;
    vec3 normal;
}
prev;

const float pbr_material = 1.0f;

void main()
{
    albedo = vec4(albedo_color, roughness);
    reflection = vec4(refl_color, ior);
    normal = vec4(normalize(prev.normal), 0.0f);
    position = vec4(prev.pos, pbr_material);
}


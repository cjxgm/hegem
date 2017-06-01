#version 450 core

layout(location=2) uniform vec3 albedo_color;

layout(location=0) out vec3 albedo;
layout(location=1) out vec3 normal;
layout(location=2) out vec3 position;
layout(location=3) out int material;

in eval_frag
{
    vec3 pos;
    vec3 normal;
}
prev;

void main()
{
    albedo = albedo_color;
    normal = normalize(prev.normal);
    position = prev.pos;
    material = 1;
}


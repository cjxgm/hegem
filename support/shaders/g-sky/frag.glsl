#version 450 core

layout(location=0) uniform vec3 sky_color;

layout(location=0) out vec3 albedo;
layout(location=1) out vec3 normal;
layout(location=2) out vec3 position;
layout(location=3) out int material;

void main()
{
    albedo = sky_color;
    normal = vec3(0, 0, 0);
    position = vec3(0, 0, 0);
    material = 0;
}


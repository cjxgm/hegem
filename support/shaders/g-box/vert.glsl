#version 450 core

layout(location=2) uniform float hotness;

out vert_geom
{
    vec4 color;
}
next;

const vec4 cold_color = vec4(7.0f, 8.0f, 10.0f, 0.005);
const vec4 warm_color = vec4(10.0f, 4.0f, 1.0f, 0.01);
const vec4 hot_color = vec4(10.0f, 0.0f, 0.0f, 0.2);

void main()
{
    gl_Position = vec4(0, 0, 0, 1);
    next.color = mix(mix(cold_color, warm_color, clamp(hotness / 5.0f, 0.0f, 1.0f)), hot_color, clamp(hotness / 10.0f, 0.0f, 1.0f));
}


#version 450 core

in vec4 emission_color;
in vec2 uv;

layout(location=3) uniform int mode;    // 0: sparks; 1: wireframed.

layout(location=0) out vec4 color;

void main()
{
    if (mode == 1) {
        color = emission_color;
    } else {
        float d = length(uv) - 1.0;
        float aa = 1.0 - smoothstep(-fwidth(d), 0.0, d);
        color = vec4(emission_color.rgb, emission_color.a * aa);
    }
}


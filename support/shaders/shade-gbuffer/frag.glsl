#version 450 core

layout(location=0) uniform sampler2D albedo;
layout(location=1) uniform sampler2D normal;
layout(location=2) uniform sampler2D position;
layout(location=3) uniform usampler2D material;

layout(location=0) out vec4 combined;

void main()
{
    ivec2 p = ivec2(gl_FragCoord.xy);
    uint mat = texelFetch(material, p, 0).r;

    if (mat == 0) {
        combined = vec4(texelFetch(albedo, p, 0).rgb, 1);
    } else {
        combined = vec4(10, 0, 10, 1);
    }
}


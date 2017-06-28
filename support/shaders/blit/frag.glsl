#version 450 core

layout(location=0) uniform sampler2D combined;
layout(location=0) out vec4 color;

void main()
{
    // make sure p is in screen space (top-left origin)
    int h = textureSize(combined, 0).y;
    ivec2 p = ivec2(gl_FragCoord.x, h - gl_FragCoord.y);
    color = texelFetch(combined, p, 0);
}


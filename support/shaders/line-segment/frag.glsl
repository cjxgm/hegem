#version 450 core

layout(location=0) out vec4 combined;

in geom_frag
{
    float cap_threshold;
    vec4 color;
    vec2 uv;
}
prev;

void main()
{
    float k = prev.cap_threshold;
    float u = (max(abs(prev.uv.x), k) - k) / (1.0f - k);
    float r = length(vec2(u, prev.uv.y));
    float d = fwidth(r);
    float a = smoothstep(1.0f, 1.0f-d, r);
    combined = vec4(prev.color.rgb, prev.color.a * a);
}


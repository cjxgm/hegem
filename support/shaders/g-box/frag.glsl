#version 450 core

layout(location=0) out vec4 combined;

const float edge_width = 1.0f;

in geom_frag
{
    vec4 color;
    vec2 uv;
}
prev;

void main()
{
    vec2 d = abs(vec2(length(dFdx(prev.uv)), length(dFdy(prev.uv))));
    vec2 edgeness = 0.5f - abs(prev.uv - 0.5f);
    bool on_edge = edgeness.x <= edge_width * d.x || edgeness.y <= edge_width * d.y;
    vec4 out_color = prev.color;
    if (on_edge) out_color.a *= 3.0f;

    combined = out_color;
}


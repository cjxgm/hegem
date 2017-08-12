#version 450 core

layout(location=0) out vec4 combined;

layout(location=3) uniform vec3 cam_apex;

const float edge_width = 1.0f;

in geom_frag
{
    vec4 color;
    vec3 normal;
    vec3 pos;
    vec2 uv;
}
prev;

void main()
{
    vec2 d = abs(vec2(length(dFdx(prev.uv)), length(dFdy(prev.uv))));
    vec2 edgeness = 0.5f - abs(prev.uv - 0.5f);
    bool on_edge = edgeness.x <= edge_width * d.x || edgeness.y <= edge_width * d.y;

    vec4 out_color = prev.color;

    vec3 viewing = normalize(prev.pos - cam_apex);
    float facing = dot(viewing, normalize(prev.normal));
    facing = max(0.0f, facing);
    facing = pow(facing, 5.0f);
    out_color.a *= mix(0.1f, 1.0f, facing);
    float edge_boost = mix(3.0f, -1.0f, facing);

    if (on_edge) out_color.a *= edge_boost;

    combined = out_color;
}


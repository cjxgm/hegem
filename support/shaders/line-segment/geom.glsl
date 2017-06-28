#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices=4) out;

layout(location=0) uniform mat4 proj_view;
layout(location=1) uniform vec2 pixel_size;
layout(location=2) uniform vec3 ends[2];
layout(location=4) uniform vec4 colors[2];
layout(location=6) uniform float width;

out geom_frag
{
    float cap_threshold;
    vec4 color;
    vec2 uv;
}
next;

const vec2 uvs[4] = {
    vec2(-1, -1),
    vec2(-1,  1),
    vec2( 1, -1),
    vec2( 1,  1),
};

void main()
{
    vec2 ps = pixel_size * 2.0f;
    vec4 end0 = proj_view * vec4(ends[0], 1.0f);
    vec4 end1 = proj_view * vec4(ends[1], 1.0f);
    end0 /= end0.w;
    end1 /= end1.w;

    vec2 size = width * 0.5f * pixel_size;
    vec2 xdir = normalize(end1.xy - end0.xy) * size;
    vec2 ydir = normalize(vec2(-xdir.y, xdir.x)) * size;

    float len = length(end1.xy - end0.xy);
    float cap_threshold = len / (length(xdir) * 2.0f + len);

    for (int i=0; i<4; i++) {
        vec2 uv = uvs[i];
        float u = uv.x * 0.5f + 0.5f;

        vec4 color = mix(colors[0], colors[1], u);
        vec3 pos = mix(end0, end1, u).xyz;
        vec2 delta = xdir * uv.x + ydir * uv.y;

        gl_Position = vec4(pos.xy + delta, pos.z, 1.0f);
        next.cap_threshold = cap_threshold;
        next.color = color;
        next.uv = uv;
        EmitVertex();
    }
    EndPrimitive();
}


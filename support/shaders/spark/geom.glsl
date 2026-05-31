#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices=4) out;

layout(location=2) uniform mat4 proj;

vec3 view_pos = vec3(0.0);
float radius = 0.0;

in vec4 instance_emission_color[];

out vec2 uv;
out vec4 emission_color;

void emit(float u, float v)
{
    uv = vec2(u, v);
    emission_color = instance_emission_color[0];
    gl_Position = proj * vec4(view_pos + vec3(radius * vec2(u, v), 0.0), 1.0);
    EmitVertex();
}

void main()
{
    view_pos = gl_in[0].gl_Position.xyz;
    radius = gl_in[0].gl_Position.w;

    emit(-1.0, -1.0);
    emit(-1.0, +1.0);
    emit(+1.0, -1.0);
    emit(+1.0, +1.0);
    EndPrimitive();
}


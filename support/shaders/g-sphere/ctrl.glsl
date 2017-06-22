#version 450 core

layout(vertices=4) out;

layout(location=0) uniform mat4 proj_view;
layout(location=1) uniform mat4 model;

in vert_ctrl
{
    vec3 pos;
}
prev[];

out ctrl_eval
{
    vec3 pos;
}
next[];

const float coverity = 0.001f;

void main()
{
    next[gl_InvocationID].pos = prev[gl_InvocationID].pos;

    if (gl_InvocationID == 0) {
        vec2 clip_pos[4];
        for (int i=0; i<4; i++) {
            vec4 p = proj_view * model * vec4(prev[i].pos, 1.0f);
            clip_pos[i] = p.xy / p.w;
        }
        vec2 min_pos = min(min(clip_pos[0], clip_pos[1]), min(clip_pos[2], clip_pos[3]));
        vec2 max_pos = max(max(clip_pos[0], clip_pos[1]), max(clip_pos[2], clip_pos[3]));
        vec2 deltas = max_pos - min_pos;
        float max_len = max(deltas.x, deltas.y);
        float subdiv = max(max_len / sqrt(coverity), 2.0f);

        gl_TessLevelInner[0] = subdiv;
        gl_TessLevelInner[1] = subdiv;
        gl_TessLevelOuter[0] = subdiv;
        gl_TessLevelOuter[1] = subdiv;
        gl_TessLevelOuter[2] = subdiv;
        gl_TessLevelOuter[3] = subdiv;
    }
}


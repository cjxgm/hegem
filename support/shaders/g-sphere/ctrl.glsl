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

const float coverity = 0.0015f;
const vec3 samples[] = {
    vec3(-1.0f, -1.0f, -1.0f),
    vec3( 1.0f, -1.0f, -1.0f),
    vec3(-1.0f,  1.0f, -1.0f),
    vec3( 1.0f,  1.0f, -1.0f),
    vec3(-1.0f, -1.0f,  1.0f),
    vec3( 1.0f, -1.0f,  1.0f),
    vec3(-1.0f,  1.0f,  1.0f),
    vec3( 1.0f,  1.0f,  1.0f),
};

void main()
{
    next[gl_InvocationID].pos = prev[gl_InvocationID].pos;

    if (gl_InvocationID == 0) {
        mat4 model2clip = proj_view * model;
        vec4 pcenter = model2clip * vec4(0.0f, 0.0f, 0.0f, 1.0f);
        float len_samples[8];
        for (int i=0; i<8; i++) {
            vec4 psample = model2clip * vec4(samples[i], 1.0f);
            vec3 dsample = psample.xyz / psample.w - pcenter.xyz / pcenter.w;
            len_samples[i] = length(dsample);
        }
        float max_len = max(
                max(
                    max(len_samples[0], len_samples[1]),
                    max(len_samples[2], len_samples[3])),
                max(
                    max(len_samples[4], len_samples[5]),
                    max(len_samples[6], len_samples[7])));
        float subdiv = ceil(max(max_len / sqrt(coverity), 3.0f));

        gl_TessLevelInner[0] = subdiv;
        gl_TessLevelInner[1] = subdiv;
        gl_TessLevelOuter[0] = subdiv;
        gl_TessLevelOuter[1] = subdiv;
        gl_TessLevelOuter[2] = subdiv;
        gl_TessLevelOuter[3] = subdiv;
    }
}


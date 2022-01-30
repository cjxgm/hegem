#version 450 core

#ifndef KUL_SPARK
    #define KUL_SPARK
    #define KUL_POS_X 0
    #define KUL_POS_Y 0
    #define KUL_POS_Z 0
    #define KUL_RADIUS 1
    #define KUL_EMITTING_COLOR_R 1
    #define KUL_EMITTING_COLOR_G 1
    #define KUL_EMITTING_COLOR_B 1
    #define KUL_OPACITY 1
#endif

layout(location=0) uniform float time;
layout(location=1) uniform mat4 model_view;

out vec4 instance_emission_color;

float kul_mod(float a, float b) { return (b == 0.0 ? 0.0 : mod(mod(a, b) + b, b)); }
float kul_div(float a, float b) { return (b == 0.0 ? 0.0 : a / b); }

float kul_cos(float a) { return cos(radians(a)); }
float kul_sin(float a) { return sin(radians(a)); }

float kul_easeout(float t, float target, float steepness) { return (t <= 0.0 ? 0.0 : target * (1.0 - pow(pow(2.0, steepness) + 1.0, -t))); }

void main()
{
    float kul_time = time;
    float kul_ID = float(gl_InstanceID);

    gl_Position.x = KUL_POS_X;
    gl_Position.y = KUL_POS_Y;
    gl_Position.z = KUL_POS_Z;
    gl_Position.w = KUL_RADIUS;
    instance_emission_color.r = KUL_EMITTING_COLOR_R;
    instance_emission_color.g = KUL_EMITTING_COLOR_G;
    instance_emission_color.b = KUL_EMITTING_COLOR_B;
    instance_emission_color.a = KUL_OPACITY;

    gl_Position.xyz = (model_view * vec4(gl_Position.xyz, 1.0)).xyz;
}


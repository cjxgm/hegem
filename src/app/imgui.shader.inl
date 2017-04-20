R"vertex-shader(
#version 450 core

layout(location=0) in vec2 pos;
layout(location=1) in vec2 uv;
layout(location=2) in vec4 color;

out vert_frag
{
    vec2 uv;
    vec4 color;
}
next;

layout(location=0) uniform mat4 proj;

void main()
{
    next.uv = uv;
    next.color = color;
    gl_Position = proj * vec4(pos, 0, 1);
}
)vertex-shader",
//.........................................................................
R"fragment-shader(
#version 450 core

in vert_frag
{
    vec2 uv;
    vec4 color;
}
prev;

out vec4 color;

layout(location=1) uniform sampler2D tex;
layout(location=2) uniform int mode;
layout(location=3) uniform vec3 hdr_blackpoint;
layout(location=4) uniform vec3 hdr_whitepoint;
layout(location=5) uniform float time;
layout(location=6) uniform float dither_amount;

float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

// uniform over [a, b], where a=0 and b=1
//     mean = (b-a)    /  2 = 1 /  2
// variance = (b-a)**2 / 12 = 1 / 12
float uniform_dist(vec2 p)
{
    return hash(p + 0.07 * time);
}

vec3 aces_film_tonemap(vec3 x)
{
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;
    x = max(x, vec3(0));
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), vec3(0), vec3(1));
}

vec3 tonemap(vec3 color, vec3 black, vec3 white)
{
    return aces_film_tonemap((color - black) / (white - black));
}

void main()
{
    vec4 mask = vec4(1, 0, 1, 1);
    if (mode == 0) {
        float a = texture(tex, prev.uv).r;
        mask = vec4(1, 1, 1, a);
    } else if (mode == 1) {
        vec3 src = texture(tex, prev.uv).rgb;
        src = tonemap(src, hdr_blackpoint, hdr_whitepoint);
        float dither = (uniform_dist(prev.uv) - 0.5) * dither_amount / 255.0;
        mask = vec4(src + dither, 1);
    }
    color = prev.color * mask;
}
)fragment-shader",


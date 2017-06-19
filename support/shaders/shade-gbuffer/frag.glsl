#version 450 core

layout(location=0) uniform sampler2D albedo;
layout(location=1) uniform sampler2D normal;
layout(location=2) uniform sampler2D position;
layout(location=3) uniform usampler2D material;
layout(location=4) uniform vec3 camera_apex;
layout(location=5) uniform vec3 sky_color;
layout(location=6) uniform int sun_lamp_count;
layout(location=7) uniform int omni_lamp_count;
layout(location=32) uniform vec3 sun_lamp_dirs[32];
layout(location=64) uniform vec3 sun_lamp_colors[32];
layout(location=96) uniform vec3 omni_lamp_centers[32];
layout(location=128) uniform vec3 omni_lamp_colors[32];

layout(location=0) out vec4 combined;

const vec3 ground_color = vec3(0.0f, 0.0f, 0.0f);

float fresnel_schlick(float ior, vec3 viewing, vec3 normal)
{
    float base = (ior - 1.0f) / (ior + 1.0f);
    base *= base;
    float ex = pow(1.0f - abs(dot(viewing, normal)), 5.0f);
    return base + (1.0f - base) * ex;
}

void main()
{
    // make sure p is in screen space (top-left origin)
    ivec2 tex_size = textureSize(albedo, 0);
    ivec2 sp = ivec2(gl_FragCoord.x, tex_size.y - gl_FragCoord.y);

    uint mat = texelFetch(material, sp, 0).r;

    // solid color
    if (mat == 0) {
        combined = vec4(texelFetch(albedo, sp, 0).rgb, 1.0f);
        return;
    }

    // PBR
    if (mat == 1) {
        vec3 a = texelFetch(albedo, sp, 0).rgb;
        vec3 n = texelFetch(normal, sp, 0).xyz;
        vec3 p = texelFetch(position, sp, 0).xyz;

        vec3 towards_lamp_dir = normalize(vec3(-2.0f, 2.0f, 1.0f));
        vec3 lamp_color = vec3(1.0f, 1.0f, 1.0f) * 10.0f;
        vec3 refl_color = vec3(1.0f, 1.0f, 1.0f);
        float roughness = 0.2f;
        float ior = 1.45;

        vec3 viewing = normalize(p - camera_apex);
        vec3 reflected = reflect(viewing, n);

        float groundness = max(dot(reflected, vec3(0.0f, -1.0f, 0.0f)), 0.0f);
        vec3 ground_color = vec3(0.0f, 0.0f, 0.0f);
        vec3 dome_color = mix(sky_color, ground_color, groundness);

        // FIXME: Temp. use Blinn-phong
        float nl = max(dot(n, towards_lamp_dir), 0.0f);
        vec3 diffuse = a * nl;

        vec3 halfvl = normalize(towards_lamp_dir - viewing);
        float nh = max(dot(n, halfvl), 0.0f);     // FIXME: is this `max` necessary?
        float r = (1.0f / (roughness + 1.0f) - 0.5f) * 100.0f;
        diffuse += refl_color * pow(nh, r) * nl;
        diffuse *= lamp_color;

        float fresnel = fresnel_schlick(ior, viewing, n);
        vec3 refl = refl_color * dome_color;
        vec3 shaded = mix(diffuse, refl, fresnel);

        combined = vec4(shaded, 1.0f);
        return;
    }

    // sky
    if (mat == 2) {
        vec3 a = texelFetch(albedo, sp, 0).rgb;
        vec3 n = texelFetch(normal, sp, 0).xyz;

        float groundness = max(dot(n, vec3(0.0f, -1.0f, 0.0f)), 0.0f);
        vec3 dome_color = mix(sky_color, ground_color, groundness);

        combined = vec4(dome_color, 1.0f);
        return;
    }

    // invalid material
    combined = vec4(10.0f, 0.0f, 10.0f, 1.0f);
}


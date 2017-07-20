#version 450 core

layout(location=0) uniform sampler2D albedo;
layout(location=1) uniform sampler2D reflection;
layout(location=2) uniform sampler2D normal;
layout(location=3) uniform sampler2D position;
layout(location=4) uniform vec3 camera_apex;
layout(location=5) uniform vec3 sky_color;
layout(location=6) uniform int sun_lamp_count;
layout(location=7) uniform int omni_lamp_count;
layout(location=32) uniform vec3 sun_lamp_dirs[32];
layout(location=64) uniform vec3 sun_lamp_colors[32];
layout(location=96) uniform vec3 omni_lamp_centers[32];
layout(location=128) uniform vec3 omni_lamp_colors[32];

layout(location=0) out vec4 combined;

const vec3 ground_dir = vec3(0.0f, -1.0f, 0.0f);
const vec3 ground_color = vec3(0.0f, 0.0f, 0.0f);

const float solid_color_material = 0.0f;
const float pbr_material = 1.0f;
const float sky_material = 2.0f;

const float pi = 3.14159265358979f;
const float clamp_eps = 1e-7f;

float fresnel_schlick(float ior, vec3 viewing, vec3 normal)
{
    float f0 = pow((ior - 1.0f) / (ior + 1.0f), 2.0f);
    float ex = pow(1.0f - abs(dot(viewing, normal)), 5.0f);
    return f0 + (1.0f - f0) * ex;
}

float dot_clamp(vec3 a, vec3 b)
{
    return clamp(dot(a, b), clamp_eps, 1.0f - clamp_eps);
}

vec3 sample_sky(vec3 towards_sky)
{
    vec3 sun_color = vec3(0.0f);
    for (int i=0; i<32; i++) {
        if (i == sun_lamp_count) break;
        vec3 dir = sun_lamp_dirs[i];
        vec3 color = sun_lamp_colors[i];

        float d = max(-dot(towards_sky, dir), 0.0f);
        float strength = pow(d, 10.0f) * 0.1f
            + pow(d, 50.0f) * 0.5f
            + pow(d, 600.0f) * 3.0f
            + smoothstep(0.4f, 0.5f, pow(d, 500.0f)) * 3.0f;
        sun_color += color * strength;
    }

    float groundness = max(dot(towards_sky, ground_dir), 0.0f);
    vec3 dome_color = mix(sky_color + sun_color, ground_color, groundness);

    return dome_color;
}

vec3 sample_diffuse_for_lamp(
        vec3 a, vec3 n, vec3 viewing, vec3 refl_color,
        float roughness, float ior,
        vec3 towards_lamp_dir)
{
    vec3 v = -viewing;
    vec3 halfvl = normalize(towards_lamp_dir + v);
    float nl = dot_clamp(n, towards_lamp_dir);
    float nv = dot_clamp(n, v);
    float nh = dot_clamp(n, halfvl);
    float vh = dot_clamp(v, halfvl);
    float nh2 = nh * nh;
    float nh4 = nh2 * nh2;
    float sane_roughness = clamp(roughness, 0.00001f, 0.99999f);
    float slope = tan(sane_roughness * (pi / 2.0f));
    float slope2 = slope * slope;

    // Lambertian
    vec3 diffuse = a / pi * nl;

    // Beckmann distribution
    float distribution = exp((nh2 - 1.0f) / (slope2 * nh2)) / (slope2 * nh4 * pi);

    // Cook Torrance geometry
    float geometry = min(1.0f, 2.0f * nh / vh * min(nv, nl));

    float microfacet_fresnel = fresnel_schlick(ior, v, halfvl);
    float spec = distribution * geometry * microfacet_fresnel / (4.0f * nv);
    vec3 specular = refl_color * spec;

    return diffuse + specular;
}

vec3 sample_diffuse(
    vec3 a, vec3 n, vec3 p,
    vec3 viewing, vec3 refl_color,
    float roughness, float ior)
{
    vec3 diffuse = vec3(0.0f);

    // accumulate diffuse from sun lamps
    for (int i=0; i<32; i++) {
        if (i == sun_lamp_count) break;
        vec3 dir = -sun_lamp_dirs[i];
        vec3 color = sun_lamp_colors[i];

        diffuse += sample_diffuse_for_lamp(a, n, viewing, refl_color, roughness, ior, dir) * color;
    }

    // accumulate diffuse from omni lamps
    for (int i=0; i<32; i++) {
        if (i == omni_lamp_count) break;
        vec3 center = omni_lamp_centers[i];
        vec3 color = omni_lamp_colors[i];

        vec3 obj_to_lamp = center - p;
        float dist = length(obj_to_lamp) + 1;
        color /= dist * dist;
        vec3 dir = normalize(obj_to_lamp);

        diffuse += sample_diffuse_for_lamp(a, n, viewing, refl_color, roughness, ior, dir) * color;
    }

    return diffuse;
}

void main()
{
    ivec2 sp = ivec2(gl_FragCoord.xy);
    vec4 pos_mat = texelFetch(position, sp, 0);
    vec3 p = pos_mat.xyz;
    float mat = pos_mat.w;

    if (mat == solid_color_material) {
        combined = vec4(texelFetch(albedo, sp, 0).rgb, 1.0f);
        return;
    }

    if (mat == pbr_material) {
        vec4 albedo_roughness = texelFetch(albedo, sp, 0);
        vec4 refl_ior         = texelFetch(reflection, sp, 0);
        vec3 n                = texelFetch(normal, sp, 0).xyz;

        vec3 a          = albedo_roughness.rgb;
        float roughness = albedo_roughness.w;

        vec3 refl_color = refl_ior.rgb;
        float ior       = refl_ior.w;

        vec3 towards_lamp_dir = normalize(vec3(-2.0f, 2.0f, 1.0f));
        vec3 viewing = normalize(p - camera_apex);
        vec3 reflected = reflect(viewing, n);

        vec3 dome_color = sample_sky(reflected);
        vec3 diffuse = sample_diffuse(a, n, p, viewing, refl_color, roughness, ior);

        float fresnel = fresnel_schlick(ior, viewing, n);
        vec3 refl = refl_color * dome_color;
        vec3 shaded = mix(diffuse, refl, fresnel);

        combined = vec4(shaded, 1.0f);
        return;
    }

    if (mat == sky_material) {
        vec3 towards_sky = texelFetch(normal, sp, 0).xyz;
        vec3 dome_color = sample_sky(towards_sky);
        combined = vec4(dome_color, 1.0f);
        return;
    }

    // invalid material
    combined = vec4(10.0f, 0.0f, 10.0f, 1.0f);
}


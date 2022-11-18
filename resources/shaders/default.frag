#version 330 core

in vec3 vertex_pos_world; // from the vertex shader
in vec3 vertex_norm_world; // from the vertex shader

out vec4 output_color; // total illumination output color

// TODO: light sources
const int MAX_LIGHTS = 8;
uniform int light_types[8]; // light types -> point=0, directional=1, spot=2
uniform vec3 light_dirs[8]; // light directions
uniform vec3 light_positions[8]; // light positions
uniform vec3 light_colors[8]; // light intensities
//uniform float light_penumbras[8]; // only for spot

// for ambient lighting
uniform float k_a;
uniform vec3 cAmbient;

// for diffuse lighting
uniform float k_d;
uniform vec3 cDiffuse;

// for specular lighting
uniform float shininess;
uniform float k_s;
uniform vec3 cam_pos;
uniform vec3 cSpecular;

// ambient
vec3 Ambient() {
    return k_a * cAmbient;
}

// lambertian/diffuse
vec3 Diffuse(float NdotL) {
    return k_d * NdotL * cDiffuse;
}

// specular
vec3 Specular(float RdotV) {
    if (shininess > 0) {
        return k_s * pow(RdotV, shininess) * cSpecular;
    } else {
        return k_s * RdotV * cSpecular;
    }

}

vec3 Phong() {

    vec3 illum = vec3(0.0);

    illum += Ambient();

    vec3 N = normalize(vertex_norm_world);
    for (int i = 0; i < MAX_LIGHTS; ++i) {

        float Fatt = 1.f;

        // direciton to light differs between light types
        vec3 L = vec3(0.f);

        if (light_types[i] == 1) {
            L = normalize(-1 * light_dirs[i]);
        } else {
            continue;
        }

        // for diffuse
//        switch(light_types[i]) {
////        case 0:
////             L = normalize(light_positions[i] - vertex_pos_world);
////            break;
//        case 1: // direction
//            L = normalize(-1 * light_dirs[i]);
//            break;
//        default:
//            continue; // ignore other light types for now
//        }

        float NdotL = dot(N, L);

        // for specular
        vec3 R = reflect(L, N);
        vec3 dirToCamera = normalize(vertex_pos_world - cam_pos);
        float RdotV = clamp(dot(R, dirToCamera), 0.0, 1.0);

        illum += Fatt * light_colors[i] * (Diffuse(clamp(NdotL, 0.0, 1.0)) + Specular(RdotV));
    }

    return illum;
}

void main() {
//    output_color = vec4(Phong(), 1.f);
    output_color = vec4(0.5f);
}

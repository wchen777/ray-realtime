#version 330 core

in vec3 vertex_pos_world; // from the vertex shader
in vec3 vertex_norm_world; // from the vertex shader

out vec4 output_color; // total illumination output color

// TODO: light sources
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

vec3 Diffuse() {

}

vec3 Phong() {
    vec3 illum = vec3(0.0);


    return illum;
}

void main() {

}

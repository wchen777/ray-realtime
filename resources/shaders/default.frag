#version 330 core

in vec3 vertex_pos_world; // from the vertex shader
in vec3 vertex_norm_world; // from the vertex shader

out vec4 output_color; // total illumination output color

// TODO: light sources

// for ambient lighting
uniform float k_a;

// for diffuse lighting
uniform float k_d;
uniform vec3 light_pos;

// for specular lighting
uniform float shininess;
uniform float k_s;
uniform vec3 cam_pos;


void main() {

}

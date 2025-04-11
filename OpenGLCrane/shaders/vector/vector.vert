#version 330 core
layout(location = 0) in vec3  in_start;
layout(location = 1) in vec3  in_direction;
layout(location = 2) in vec4  in_color;
layout(location = 3) in float in_length;

out VS_OUT {
    vec3 start;
    vec3 direction;
    vec4 color;
    float length;
} vs_out;

void main() {
    vs_out.start = in_start;
    vs_out.direction = in_direction;
    vs_out.length = in_length;
    vs_out.color = in_color;
}

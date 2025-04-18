#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out FragData {
    vec3 frag_pos;  
    vec3 frag_norm;
} vs_out;

uniform mat4 model;
uniform mat4 view;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(model)));

    gl_Position = view * model * vec4(position, 1.0);

    vs_out.frag_pos = vec3(model * vec4(position, 1.0));
    vs_out.frag_norm = normalize(normalMatrix * normal);
}
#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;


void main()
{
    gl_Position = projection * view * model * vec4(inPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
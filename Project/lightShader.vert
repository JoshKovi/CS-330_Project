#version 440 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //Vert shader is basic for the lighting elements
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
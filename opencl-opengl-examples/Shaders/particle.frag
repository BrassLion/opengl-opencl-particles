#version 330 core

in vec3 fragment_velocity;

out vec4 color;

void main()
{
//    color = vec4(1.0);
    color = vec4(fragment_velocity, 1.0);
}
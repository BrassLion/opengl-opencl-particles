#version 330 core

in vec2 fragment_uv;

out vec4 color;

void main()
{
    color = vec4(fragment_uv, 0.0, 1.0);
}
#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 mvpMatrix;
uniform mat4 model_matrix;

uniform vec4 camera_world_position;

out vec3 fragment_world_position;

void main()
{
    fragment_world_position = vec3(model_matrix * vec4(position, 0.0));
    
    gl_Position = mvpMatrix * vec4(position, 1.0);
}
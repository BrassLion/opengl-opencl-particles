#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 velocity;

uniform mat4 mvpMatrix;
uniform mat4 model_matrix;

uniform vec4 camera_world_position;

out vec3 fragment_world_position;
out vec3 fragment_velocity;

void main()
{
    fragment_world_position = vec3(model_matrix * position);
    fragment_velocity = velocity.xyz;
    
    gl_Position = mvpMatrix * position;
    gl_PointSize = 5.0;
}

#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 velocity;
layout (location = 2) in vec2 life;

uniform mat4 mvpMatrix;
uniform mat4 model_matrix;

uniform vec4 camera_world_position;

out vec3 fragment_world_position;
out vec3 fragment_velocity;
out vec2 fragment_particle_life;

void main()
{
    fragment_world_position = vec3(model_matrix * position);
    fragment_velocity = velocity.xyz;
    fragment_particle_life = life;
    
    gl_Position = mvpMatrix * position;
    gl_PointSize = 2.0;
}

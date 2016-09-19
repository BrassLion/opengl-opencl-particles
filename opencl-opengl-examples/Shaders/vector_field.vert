#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;

uniform mat4 mvpMatrix;
uniform mat4 model_matrix;

uniform vec4 camera_world_position;

out vec3 geometry_world_position;
out vec3 geometry_uv;

void main()
{
    geometry_world_position = vec3(model_matrix * vec4(position, 0.0));
    geometry_uv = uv; 
 
    gl_Position = mvpMatrix * vec4(position, 1.0);
    gl_PointSize = 5.0;
}

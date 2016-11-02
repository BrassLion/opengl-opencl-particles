#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;

uniform mat4 mvpMatrix;
uniform mat4 model_matrix;

uniform vec4 camera_world_position;
uniform int field_sample_points_z;

out vec3 tesc_world_position;
out vec3 tesc_uv;
out float tesc_patch_depth;

void main()
{
    tesc_world_position = vec3(model_matrix * vec4(position, 0.0));
    tesc_uv = uv;
    tesc_patch_depth = float(gl_InstanceID) / float(field_sample_points_z);
    
    gl_Position = mvpMatrix * vec4(position, 1.0);
}

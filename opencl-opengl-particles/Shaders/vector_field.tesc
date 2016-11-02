#version 410 core

layout(vertices = 8) out;

in vec3 tesc_world_position[];
in vec3 tesc_uv[];
in float tesc_patch_depth[];

uniform int field_sample_points_x;
uniform int field_sample_points_y;

out vec3 tese_world_position[];
out vec3 tese_uv[];
patch out float tese_patch_depth;

void main()
{
    tese_world_position[gl_InvocationID] = tesc_world_position[gl_InvocationID];
    tese_uv[gl_InvocationID] = tesc_uv[gl_InvocationID];
    tese_patch_depth = tesc_patch_depth[0];
    
    gl_TessLevelOuter[0] = field_sample_points_y;
    gl_TessLevelOuter[1] = field_sample_points_x;
    gl_TessLevelOuter[2] = field_sample_points_y;
    gl_TessLevelOuter[3] = field_sample_points_x;

    gl_TessLevelInner[0] = field_sample_points_x;
    gl_TessLevelInner[1] = field_sample_points_y;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
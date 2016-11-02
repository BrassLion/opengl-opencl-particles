#version 410

layout(points) in;
layout(line_strip, max_vertices = 3) out;

in vec3 geom_world_position[];
in vec3 geom_uv[];

uniform mat4 mvpMatrix;
uniform sampler3D tex;

out vec3 fragment_world_position;
out vec3 fragment_uv;

void main()
{
    fragment_uv = geom_uv[0];
    fragment_world_position = geom_world_position[0];
    
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    
    vec3 voxel_force = texture(tex, geom_uv[0]).xyz * 0.01;
    float voxel_force_magnitude = length(voxel_force);
    
    vec4 vector_point_pos = gl_in[0].gl_Position + mvpMatrix * vec4(voxel_force, 0.0);
    
    gl_Position = vector_point_pos;
    EmitVertex();
    
    vec4 vector_arrow_tip = gl_in[0].gl_Position + mvpMatrix * vec4(0.0, min(voxel_force_magnitude, 0.5), 0.0, 0.0);
    
    gl_Position = mix(vector_point_pos, vector_arrow_tip, 0.1);
    EmitVertex();
    
//    gl_Position = gl_in[0].gl_Position + vec4(0.5,0.0,0.0,0.0);
//    EmitVertex();
    
    EndPrimitive();
}

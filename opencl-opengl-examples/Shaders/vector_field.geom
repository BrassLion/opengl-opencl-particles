#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec3 geometry_world_position[];
in vec3 geometry_uv[];

uniform mat4 mvpMatrix;
uniform sampler3D tex;

out vec3 fragment_world_position;
out vec3 fragment_uv;

void main()
{
    fragment_uv = geometry_uv[0];
    fragment_world_position = geometry_world_position[0];
    
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + mvpMatrix * vec4(texture(tex, geometry_uv[0]).xyz, 0.0);
    EmitVertex();
    
    EndPrimitive();
}
#version 410

layout(points) in;
layout(line_strip, max_vertices = 2) out;

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
    
    gl_Position = gl_in[0].gl_Position + mvpMatrix * vec4(texture(tex, geom_uv[0]).xyz * 0.01, 0.0);
    EmitVertex();
    
//    gl_Position = gl_in[0].gl_Position + vec4(0.5,0.0,0.0,0.0);
//    EmitVertex();
    
    EndPrimitive();
}
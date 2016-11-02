#version 410 core

layout(quads, point_mode) in;

in vec3 tese_world_position[];
in vec3 tese_uv[];
patch in float tese_patch_depth;

out vec3 geom_world_position;
out vec3 geom_uv;

//quad interpol
vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
    vec4 a = mix(v0, v1, gl_TessCoord.x);
    vec4 b = mix(v3, v2, gl_TessCoord.x);
    return mix(a, b, gl_TessCoord.y);
}

vec3 interpolate(in vec3 v0, in vec3 v1, in vec3 v2, in vec3 v3)
{
    vec3 a = mix(v0, v1, gl_TessCoord.x);
    vec3 b = mix(v3, v2, gl_TessCoord.x);
    return mix(a, b, gl_TessCoord.y);
}

void main()
{
    geom_world_position = tese_world_position[1];
    geom_uv = mix(
                  interpolate(tese_uv[0], tese_uv[1], tese_uv[2], tese_uv[3]),
                  interpolate(tese_uv[4], tese_uv[5], tese_uv[6], tese_uv[7]),
                              tese_patch_depth);
    
    gl_Position = mix(interpolate(
                                  gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position),
                      interpolate(
                                  gl_in[4].gl_Position, gl_in[5].gl_Position, gl_in[6].gl_Position, gl_in[7].gl_Position),
                      tese_patch_depth);
}
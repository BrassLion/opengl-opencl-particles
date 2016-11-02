#version 330 core

in vec3 fragment_velocity;
in vec2 fragment_particle_life;

out vec4 color;

void main()
{
//    color = vec4(1.0, 1.0, 1.0, 0.1);
    float speed = length(fragment_velocity);
    vec3 c = mix(vec3(1.0), vec3(1.0, 0.0, 0.0), speed);
    
    color = vec4(c, 1.0f - fragment_particle_life.x / fragment_particle_life.y);
    
//    color = vec4(1.0f - fragment_particle_life.x / fragment_particle_life.y, 0.0, 0.0, 1.0);
}

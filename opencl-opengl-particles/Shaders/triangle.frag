#version 330 core

in vec3 fragment_world_position;
in vec2 fragment_uv;

uniform mat4 model_matrix;

uniform vec4 camera_world_position;

uniform sampler2D tex;

out vec4 color;

#define STEPS 64
#define STEP_SIZE 0.05

bool sphereHit (vec3 p)
{
    return distance(p, vec3(0.0) ) < 1.0;
}

bool raycastHit (vec3 position, vec3 direction)
{
    for (int i = 0; i < STEPS; i++)
    {
        if ( sphereHit(position) )
            return true;
        
        position += direction * STEP_SIZE;
    }
    
    return false;
}

void main()
{
    vec3 worldPosition = fragment_world_position;
    vec3 viewDirection = normalize(fragment_world_position - camera_world_position.xyz);
    
    if ( raycastHit(worldPosition, viewDirection) )
        color = vec4(1,0,0,1); // Red if hit the ball
    else
        color = vec4(1,1,1,1); // White otherwise
    
    color = texture(tex, fragment_uv);
}
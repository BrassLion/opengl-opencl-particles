
struct __attribute__ ((packed)) BoundingBox {
    float4 corner1;
    float4 corner2;
};

struct __attribute__ ((packed)) Particle {
    float4 pos;
    float4 vel;
    float2 life;
};

const sampler_t vector_field_sampler = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

int is_between(float min, float max, float value)
{
    return isgreaterequal(max, value) * islessequal(min, value);
}

uint MWC64X(__global uint2 *state)
{
    enum { A=4294883355U};
    uint x=(*state).x, c=(*state).y;  // Unpack the state
    uint res=x^c;                     // Calculate the result
    uint hi=mul_hi(x,A);              // Step the RNG
    x=x*A+c;
    c=hi+(x<c);
    *state=(uint2)(x,c);               // Pack the state back up
    return res;                       // Return the next result
}

inline float rand_float(__global uint2* rvec, float min, float max) {
    float rand = (float)(MWC64X(rvec)) / (float)(0xFFFFFFFF);
    
    return min + rand * (max - min);
}

__kernel void particle_simulation(__global struct Particle* particles, __global uint2* rng_seeds, __read_only image3d_t vector_field, __constant struct BoundingBox* bounding_box, float tightness, float time)
{
    unsigned int x = get_global_id(0);
    unsigned int y = get_global_id(1);
    unsigned int w = get_global_size(0)-1;
    unsigned int h = get_global_size(1)-1;
    
    unsigned int i = x + y * w;
    
    __global struct Particle *particle = &particles[i];
    
//    float3 acceleration = - (6.674f * 1.0f) / pow(length(particles[i].pos.xyz), 1) * normalize(particles[i].pos.xyz);
    
//    particles[i].pos += 0.01f;
//    particles[i].vel.xyz += acceleration * time;
//    uv[0] = (float2)(1.2f, 3.4f);
//
    if(particle->life.x == particle->life.y) {
        
        particle->pos.xyz = (float3)(rand_float(&rng_seeds[i], -1.0f, 1.0f), rand_float(&rng_seeds[i], -1.0f, 1.0f), rand_float(&rng_seeds[i], -1.0f, 1.0f));
        particle->vel.xyz = (float3)(rand_float(&rng_seeds[i], -1.0f, 1.0f), rand_float(&rng_seeds[i], -1.0f, 1.0f), rand_float(&rng_seeds[i], -1.0f, 1.0f));
        particle->life.x = 0.0f;
        return;
    }
    
    particle->pos.xyz += particle->vel.xyz * time;
    particle->life.x += time;
    
    if(particle->life.x > particle->life.y) {
        
        particle->life.x = particle->life.y;
    }
    
    float4 particle_pos_in_vector_field = particle->pos - bounding_box->corner1;
    float4 vector_field_length = bounding_box->corner2 - bounding_box->corner1;
    
    particle_pos_in_vector_field.xyz /= vector_field_length.xyz;
    
    int is_inside_vector_field = is_between(0.0f, 1.0f, particle_pos_in_vector_field.x) * is_between(0.0f, 1.0f, particle_pos_in_vector_field.y) * is_between(0.0f, 1.0f, particle_pos_in_vector_field.z);
    
//    printf("WORK %u: %u, %u, %f\n", i, rng_seeds[i].xy, rand_float(&rng_seeds[i]));
    
    if(!is_inside_vector_field)
        return;
    
    float4 voxel = (float4)(1.0f / float(get_image_width(vector_field)) / 2.0f, 1.0f / float(get_image_height(vector_field)) / 2.0f, 1.0f / float(get_image_depth(vector_field)) / 2.0f, 0.0f);
    voxel = mix(voxel, (float4)(1.0f) - voxel, particle_pos_in_vector_field);
    
    float4 acceleration = read_imagef(vector_field, vector_field_sampler, voxel);
    
    particle->vel.xyz = particle->vel.xyz * tightness + acceleration.xyz * time;
//    particle->vel.xyz += acceleration.xyz * time;
//    printf("\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n", vector_field_length, particle_pos_in_vector_field, voxel, acceleration);
}

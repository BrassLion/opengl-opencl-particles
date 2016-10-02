
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

__kernel void particle_simulation(__global struct Particle* particles, __read_only image3d_t vector_field, __constant struct BoundingBox* bounding_box, float time)
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
        
        particle->pos.xyz = (float3)(0.0f, 0.0f, 0.0f);
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
    
//    printf("\n%f, %f, %f, %f\n%u, %u, %u\n", particle_pos_in_vector_field, is_between(0.0f, 1.0f, particle_pos_in_vector_field.x), is_between(0.0f, 1.0f, particle_pos_in_vector_field.y), is_between(0.0f, 1.0f, particle_pos_in_vector_field.z));
    
    if(!is_inside_vector_field)
        return;
    
    float4 voxel = (float4)(1.0f / float(get_image_width(vector_field)) / 2.0f, 1.0f / float(get_image_height(vector_field)) / 2.0f, 1.0f / float(get_image_depth(vector_field)) / 2.0f, 0.0f);
    voxel = mix(voxel, (float4)(1.0f) - voxel, particle_pos_in_vector_field);
    
    float4 acceleration = read_imagef(vector_field, vector_field_sampler, voxel);
    
    particle->vel.xyz += acceleration.xyz * time;
//    printf("\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n", vector_field_length, particle_pos_in_vector_field, voxel, acceleration);
}
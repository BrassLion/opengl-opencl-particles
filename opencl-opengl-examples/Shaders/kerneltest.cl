//__kernel void sine_wave(__global float4* pos, unsigned int width, unsigned int height, float time)
//{
//    unsigned int x = get_global_id(0);
//    unsigned int y = get_global_id(1);
//    
//    // calculate uv coordinates
//    float u = x / (float) width;
//    float v = y / (float) height;
//    u = u*2.0f - 1.0f;
//    v = v*2.0f - 1.0f;
//    
//    // calculate simple sine wave pattern
//    float freq = 4.0f;
//    float w = sin(u*freq + time) * cos(v*freq + time) * 0.5f;
//    
//    // write output vertex
//    pos[y*width+x] = (float4)(u, w, v, 1.0f);
//}
//
//
//__kernel void texture_red(__write_only image2d_t texture)
//{
//    int x = get_global_id(0);
//    int y = get_global_id(1);
//    int w = get_global_size(0)-1;
//    int h = get_global_size(1)-1;
//    int2 coords = (int2)(x,y);
//    float red = (float)x/(float)w;
//    float blue = (float)y/(float)h;
//    float4 val = (float4)(1.0f, 0.0f, 0.0f, 1.0f);
//    
//    write_imagef(texture, coords, val);
//}

struct __attribute__ ((packed)) Particle {
    float4 pos;
    float4 vel;
    float2 life;
};

__kernel void particle_simulation(__global struct Particle* particles, float time)
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
}
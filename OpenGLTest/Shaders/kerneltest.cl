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
    float2 col;
};

__kernel void offset_test(__global struct Particle* particles)
{
    int x = get_global_id(0);
    int y = get_global_id(1);
    int w = get_global_size(0)-1;
    int h = get_global_size(1)-1;
    
//    particles[x + y * w].x += 0.01f;
    particles[x + y * w].pos += (float4)(0.01f, 0.0f, 0.0f, 0.0f);
    particles[x + y * w].col = (float2)(0.0f, 1.0f);
//    uv[0] = (float2)(1.2f, 3.4f);
}
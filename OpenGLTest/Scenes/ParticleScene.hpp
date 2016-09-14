//
//  ParticleScene.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 21/08/2016.
//
//

#ifndef ParticleScene_hpp
#define ParticleScene_hpp

#include <stdio.h>
#include <OpenCL/OpenCL.h>

#include "Scene.hpp"

class ParticleScene : public Scene
{
private:
    
    cl_command_queue m_cl_cmd_queue;
    cl_kernel m_cl_krnl;
    cl_mem m_cl_particle_buffer;
    
    void initialize_opencl(GLuint particleVBO);
    void run_opencl();
    
public:
    
    using Scene::Scene;
    
    void initialize();
    void draw();
};

#endif /* ParticleScene_hpp */

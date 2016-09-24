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
#include <nanogui/nanogui.h>

#include "Scene.hpp"
#include "Mesh.hpp"
#include "Utility.hpp"

class ParticleScene : public Scene
{
private:
        
    unsigned int m_minimum_particle_count;
    unsigned int m_maximum_particle_count;
    unsigned int m_current_particle_count;
    
    std::shared_ptr<Mesh> particleMesh;
    std::shared_ptr<Mesh> m_vector_field_mesh;
    
    cl_context m_cl_gl_context;
    cl_command_queue m_cl_cmd_queue;
    cl_kernel m_cl_krnl_particle_simulation;
    cl_kernel m_cl_krnl_new_particles;
    cl_mem m_cl_particle_buffer;
    
    void initialize_vector_field();
    void initialize_opencl();
    void initialize_gui(nanogui::Screen *gui_screen);
    
    void run_particle_simulation(float delta_time);
    
    void set_particle_count(unsigned int particle_count);
    
public:
    
    ParticleScene(int width, int height) : Scene(width, height)
    {
        m_minimum_particle_count = 1;
        m_maximum_particle_count = 1000000;
        m_current_particle_count = 1;
    }
    
    void initialize(nanogui::Screen *gui_screen);
    void draw();
};

#endif /* ParticleScene_hpp */

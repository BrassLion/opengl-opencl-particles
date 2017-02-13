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
    float m_particle_tightness;
    
    bool m_is_paused = false;
    bool m_is_rotating = false;
    double m_last_time;
    
    std::shared_ptr<Mesh> m_particle_mesh;
    std::shared_ptr<Mesh> m_vector_field_mesh;
    
    cl_context m_cl_gl_context;
    cl_command_queue m_cl_cmd_queue;
    
    cl_kernel m_cl_krnl_particle_simulation;
    
    cl_mem m_cl_particle_buffer;
    cl_mem m_cl_vector_field_texture;
    cl_mem m_cl_vector_field_bounding_box;
    cl_mem m_cl_rng_seeds;

    void initialize_vector_field();
    void initialize_opencl();
    void initialize_gui(nanogui::Screen *gui_screen);
    
    template<typename T>
    static void new_variable_slider(nanogui::Window *gui_window, std::string title, float initial_value, T min_slider_value, T max_slider_value, std::function<void(float)> callback, std::function<void(float)> final_callback)
    {
        new nanogui::Label(gui_window, title, "sans-bold");
        
        nanogui::Widget *panel = new nanogui::Widget(gui_window);
        panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                                nanogui::Alignment::Middle, 0, 20));
        
        nanogui::Slider *slider = new nanogui::Slider(panel);
        slider->setValue(initial_value);
        slider->setFixedWidth(80);
        
        nanogui::TextBox *text_box = new nanogui::TextBox(panel);
        
        slider->setCallback([=](float value) {
            
            callback(value);
            
            std::string value_string = std::to_string((T) (min_slider_value +  value * (max_slider_value - min_slider_value)));
            value_string.resize(8);
            
            text_box->setValue(value_string);
        });
        slider->setFinalCallback(final_callback);
        text_box->setFixedSize(Eigen::Vector2i(70,25));
        text_box->setFontSize(14);
        text_box->setAlignment(nanogui::TextBox::Alignment::Right);
        
        slider->callback()(initial_value);
    }
    
    void run_particle_simulation(float delta_time);
    
    void set_particle_count(unsigned int particle_count);
    
public:
    
    ParticleScene(int width, int height) : Scene(width, height)
    {
        m_minimum_particle_count = 1;
        m_maximum_particle_count = 1000000;
        m_current_particle_count = 500000;
        m_particle_tightness = 0.0f;
    }
    
    void initialize(nanogui::Screen *gui_screen);
    void draw();
    
    void mouse_callback(double xpos, double ypos);
    void key_callback(int key, int action);
};

#endif /* ParticleScene_hpp */

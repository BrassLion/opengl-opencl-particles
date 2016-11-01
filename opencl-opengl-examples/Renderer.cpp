//
//  Renderer.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 29/07/2016.
//
//

#include "Renderer.hpp"

// GLFW
#include <GLFW/glfw3.h>

void Renderer::draw(std::shared_ptr<Object> rootNode)
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    for(std::function<void ()> prerender_func : m_prerender_functions) {
        
        prerender_func();
    }
    
    m_prerender_functions.clear();
    
    for(std::shared_ptr<Viewport> viewport : m_viewports) {
        
        viewport->draw(rootNode);
    }
}

void Renderer::addViewport(std::shared_ptr<Viewport> viewport)
{
    m_viewports.push_back(viewport);
}

void Renderer::queueFunctionBeforeRender(std::function<void ()> func)
{
    m_prerender_functions.push_back(func);
}

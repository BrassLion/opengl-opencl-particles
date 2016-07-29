//
//  Renderer.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 29/07/2016.
//
//

#include "Renderer.hpp"

void Renderer::draw(Object *rootNode)
{
    for(std::function<void ()> prerender_func : m_prerender_functions) {
        
        prerender_func();
    }
    
    m_prerender_functions.clear();
    
    for(Viewport *viewport : m_viewports) {
        
        viewport->draw(rootNode);
    }
}

void Renderer::addViewport(Viewport *viewport)
{
    m_viewports.push_back(viewport);
}

void Renderer::queueFunctionBeforeRender(std::function<void ()> func)
{
    m_prerender_functions.push_back(func);
}
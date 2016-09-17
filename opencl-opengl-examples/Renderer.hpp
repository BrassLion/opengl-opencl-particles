//
//  Renderer.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 29/07/2016.
//
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include <functional>

#include <vector>

#include "Viewport.hpp"
#include "Object.hpp"

class Renderer
{
private:
    
    std::vector<std::shared_ptr<Viewport>> m_viewports;
    std::vector<std::function<void ()>> m_prerender_functions;
public:
    
    void draw(std::shared_ptr<Object> rootNode);
    void addViewport(std::shared_ptr<Viewport> viewport);
    void queueFunctionBeforeRender(std::function<void()> func);
};

#endif /* Renderer_hpp */

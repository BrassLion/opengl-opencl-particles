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
    
    std::vector<Viewport *> m_viewports;
    std::vector<std::function<void ()>> m_prerender_functions;
public:
    
    void draw(Object *rootNode);
    void addViewport(Viewport *viewport);
    void queueFunctionBeforeRender(std::function<void()> func);
};

#endif /* Renderer_hpp */

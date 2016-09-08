//
//  Viewport.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 29/07/2016.
//
//

#include "Viewport.hpp"

#include <GLFW/glfw3.h>

void Viewport::draw(std::shared_ptr<Object> rootNode)
{
    glViewport(m_x, m_y, m_width, m_height);
    
    for(std::shared_ptr<Camera> camera : m_cameras) {
        
        rootNode->draw(camera);
    }
}

void Viewport::addCamera(std::shared_ptr<Camera> camera)
{
    m_cameras.push_back(camera);
}
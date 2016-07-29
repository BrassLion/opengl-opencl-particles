//
//  Viewport.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 29/07/2016.
//
//

#ifndef Viewport_hpp
#define Viewport_hpp

#include <stdio.h>

#include <vector>

#include "Camera.hpp"
#include "Object.hpp"

class Viewport
{
private:
    
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    
    std::vector<Camera *> m_cameras;
    
public:
    
    Viewport(int x, int y, int width, int height) {
        
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
    }
    
    void draw(Object *rootNode);
    void addCamera(Camera *camera);
};

#endif /* Viewport_hpp */

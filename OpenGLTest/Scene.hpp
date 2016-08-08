//
//  Scene.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 08/08/2016.
//
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>

#include "Renderer.hpp"

class Scene {
    
private:
    Renderer *renderer;
    Object* rootNode;
    
public:
    
    Scene() {
        
        renderer = new Renderer();
        rootNode = new Object();
    }
    
    void initialize(int width, int height);
    void draw();
    
    void key_callback(int key, int action);
    void mouse_callback(double xpos, double ypos);
    void scroll_callback(double xoffset, double yoffset);
};

#endif /* Scene_hpp */

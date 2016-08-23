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
#include <string>

#include "Renderer.hpp"

class Scene {
    
private:
    
    Camera* camera;
    Object* cameraContainer;
    
    Renderer *renderer;
    Object* rootNode;
    
    // Mouse variables.
    float lastX = 400, lastY = 300;
    bool firstMouse = true;
    
protected:
    
    std::string SRC_DIR;

public:
    
    Scene(int width, int height) {
        
        SRC_DIR = "/Users/Sam/Documents/Personal/OpenGLTest/OpenGLTest";
        
        renderer = new Renderer();
        rootNode = new Object();
        
        // Camera variables.
        camera = new Camera(glm::radians(60.0f), (float)width, (float)height, 0.1f, 100.0f);
        
        // Setup viewport
        Viewport *viewport = new Viewport(0, 0, width, height);
        viewport->addCamera(camera);
        
        renderer->addViewport(viewport);
        
        cameraContainer = new Object();
        
        cameraContainer->addChild(camera);
        
        camera->setPosition(glm::vec3(0.0f,0.0f,10.0f));
    }
    
    virtual void initialize();
    void draw();
    void deinitialize();
    
    void key_callback(int key, int action);
    void mouse_callback(double xpos, double ypos);
    void scroll_callback(double xoffset, double yoffset);
};

#endif /* Scene_hpp */

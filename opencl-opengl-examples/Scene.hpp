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
#include <nanogui/nanogui.h>

#include "Renderer.hpp"
#include "ShaderReloader.hpp"

class Scene {
    
private:
    
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Object> cameraContainer;
    
    // Mouse variables.
    float lastX = 400, lastY = 300;
    bool firstMouse = true;
    bool is_mouse_down = false;
    
protected:
        
    std::unique_ptr<ShaderReloader> shaderReloader;
    std::unique_ptr<Renderer> renderer;
    std::shared_ptr<Object> rootNode;

public:
    
    Scene(int width, int height) {
        
        shaderReloader = std::unique_ptr<ShaderReloader>( new ShaderReloader() );
        renderer = std::unique_ptr<Renderer>(new Renderer());
        rootNode = std::shared_ptr<Object>(new Object());
        
        // Camera variables.
        camera = std::shared_ptr<Camera>(new Camera(glm::radians(60.0f), (float)width, (float)height, 0.1f, 1000.0f));
        
        // Setup viewport
        std::shared_ptr<Viewport> viewport(new Viewport(0, 0, width, height));
        viewport->addCamera(camera);
        
        renderer->addViewport(viewport);
        
        cameraContainer = std::shared_ptr<Object>(new Object());
        
        cameraContainer->addChild(camera);
        
        camera->setPosition(glm::vec3(0.0f,0.0f,10.0f));
    }
    
    ~Scene() {};
    
    virtual void initialize(nanogui::Screen *gui_screen);
    virtual void draw();
    void deinitialize();
    
    void key_callback(int key, int action);
    virtual void mouse_callback(double xpos, double ypos);
    void mouse_button_callback(int button, int action, int modifiers);
    void scroll_callback(double xoffset, double yoffset);
};

#endif /* Scene_hpp */

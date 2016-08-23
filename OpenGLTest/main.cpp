//
//  main.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 23/07/2016.
//
//

#include <iostream>
#include <stdio.h>
#include <sstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <nanogui/nanogui.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <map>

#include "Scene.hpp"
#include "Scenes/ParticleScene.hpp"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void button_callback(GLFWwindow* window, int button, int action, int modifiers);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int frame = 0;
double lastTime = 0;

// GUI.
nanogui::Screen* gui_screen;

std::map<std::string, Scene *> availableScenes;
Scene *currentScene;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_REFRESH_RATE, 30);
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    printf("Using OpenGL v%s\n", (char*)glGetString(GL_VERSION));
    
    int width, height;
    
    glfwGetFramebufferSize(window, &width, &height);
    
    // Init NanoGUI.
    nanogui::init();
    gui_screen = new nanogui::Screen();
    gui_screen->initialize(window, true);
    
    nanogui::Window *gui_window = new nanogui::Window(gui_screen, "Basic widgets");
    gui_window->setPosition(Eigen::Vector2i(20, 15));
    gui_window->setLayout(new nanogui::GroupLayout());
    
    nanogui::PopupButton *scenesButton = new nanogui::PopupButton(gui_window, "Scenes");
    nanogui::Popup *popup = scenesButton->popup();
    popup->setLayout(new nanogui::GroupLayout());

    nanogui::Button *b = new nanogui::Button(popup, "Basic scene");
    b->setCallback([&]{
        currentScene = new Scene(width, height);
        currentScene->initialize();
    });
    
    b = new nanogui::Button(popup, "OpenCL scene");
    b->setCallback([&]{
        currentScene = new ParticleScene(width, height);
        currentScene->initialize();
    });

    
    
    gui_screen->performLayout();
    
    currentScene = new ParticleScene(width, height);
    
    currentScene->initialize();
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Draw our first triangle
        currentScene->draw();
        
        // Draw GUI.
        gui_screen->drawContents();
        gui_screen->drawWidgets();
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
        
        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;
        
        frame++;
        if ( delta >= 1.0 ){ // If last cout was more than 1 sec ago
            
            double fps = double(frame) / delta;
            
            std::stringstream ss;
            ss << "OpenGLTest" << " " << "v0.1" << " [" << fps << " FPS]";
            
            glfwSetWindowTitle(window, ss.str().c_str());
            
            frame = 0;
            lastTime = currentTime;
        }
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    else
        currentScene->key_callback(key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    gui_screen->cursorPosCallbackEvent(xpos, ypos);
    
    currentScene->mouse_callback(xpos, ypos);
}

void button_callback(GLFWwindow* window, int button, int action, int modifiers)
{
    gui_screen->mouseButtonCallbackEvent(button, action, modifiers);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gui_screen->scrollCallbackEvent(xoffset, yoffset);
    
    currentScene->scroll_callback(xoffset, yoffset);
}
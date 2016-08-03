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

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "ShaderReloader.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Viewport.hpp"
#include "Renderer.hpp"


const std::string SRC_DIR = "/Users/Sam/Documents/Personal/OpenGLTest/OpenGLTest";

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Mouse variables.
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Camera variables.
Camera *camera = new Camera(glm::radians(60.0f), (float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
Object* cameraContainer;

int frame = 0;
double lastTime = 0;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    glfwSetScrollCallback(window, scroll_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    // Setup renderer.
    Renderer *renderer = new Renderer();
    
    // Setup viewport
    Viewport *viewport = new Viewport(0, 0, width, height);
    viewport->addCamera(camera);
    
    renderer->addViewport(viewport);
    
    // Setup scene.
    Object *rootNode = new Object();
    
    cameraContainer = new Object();
    
    cameraContainer->addChild(camera);
    
    camera->setPosition(glm::vec3(0.0f,0.0f,10.0f));
    
    Shader* triangleShader = new Shader();
    
    triangleShader->setShader(SRC_DIR + "/Shaders/triangle.vert", GL_VERTEX_SHADER);
    triangleShader->setShader(SRC_DIR + "/Shaders/triangle.frag", GL_FRAGMENT_SHADER);
    triangleShader->initialize();
    
    Mesh* triangleMesh = new Mesh();
    Mesh* triangleMesh2 = new Mesh();
    
    std::vector<GLfloat> vertices = {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left
    };
    std::vector<GLuint> indices = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };
    
    triangleMesh->initialize(vertices, indices);
    triangleMesh->setShader(triangleShader);
    triangleMesh->setPosition( glm::vec3(0.0f,0.0f,0.0f) );
    triangleMesh->setScale( glm::vec3(1.0f,1.0f,1.0f) );
    
    triangleMesh2->initialize(vertices, indices);
    triangleMesh2->setShader(triangleShader);
    triangleMesh2->setPosition( glm::vec3(0.0f,2.0f,0.0f) );
    
    triangleMesh->addChild(triangleMesh2);
    rootNode->addChild(triangleMesh);
    
    // Reload shader when it's files are changed.
    ShaderReloader::getInstance()
    .addFilesToWatch([&] {
                         
                         renderer->queueFunctionBeforeRender([&] {
                             
                             triangleShader->deleteShader();
                             triangleShader->initialize();
                         });
                     },
                     
                     SRC_DIR + "/Shaders/triangle.frag",
                     SRC_DIR + "/Shaders/triangle.vert"
                     );
    
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw our first triangle
        renderer->draw(rootNode);
        
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
    
    else if(key == GLFW_KEY_UP && action != GLFW_RELEASE)
        cameraContainer->setOrientation( cameraContainer->getOrientation() * glm::angleAxis(0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) );
    
    else if(key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
        cameraContainer->setOrientation( cameraContainer->getOrientation() * glm::angleAxis(0.5f, glm::vec3(-1.0f, 0.0f, 0.0f)) );
    
    else if(key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
        cameraContainer->setOrientation( cameraContainer->getOrientation() * glm::angleAxis(0.5f, glm::vec3(0.0f, 1.0f, 0.0f)) );

    else if(key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
        cameraContainer->setOrientation( cameraContainer->getOrientation() * glm::angleAxis(0.5f, glm::vec3(0.0f, -1.0f, 0.0f)) );
    
    else if(key == GLFW_KEY_R && action == GLFW_PRESS)
        cameraContainer->setOrientation(glm::quat());
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xpos;
    lastY = ypos;
    
    cameraContainer->setOrientation( cameraContainer->getOrientation() * glm::angleAxis(0.05f * xoffset, glm::vec3(0.0f, 1.0f, 0.0f)) );
    cameraContainer->setOrientation( cameraContainer->getOrientation() * glm::angleAxis(0.05f * yoffset, glm::vec3(1.0f, 0.0f, 0.0f)) );
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    glm::vec3 new_camera_position = camera->getPosition() + glm::vec3(0.0f, 0.0f, yoffset);

    if(new_camera_position.z < 5.0f)
        new_camera_position.z = 5.0f;
    
    if(new_camera_position.z > 50.0f)
        new_camera_position.z = 50.0f;
    
    camera->setPosition( new_camera_position );
}
//
//  main.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 23/07/2016.
//
//

#include <iostream>
#include <stdio.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "Shader.hpp";
#include "Mesh.hpp";
#include "Camera.hpp";

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Mouse variables.
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Camera variables.
Camera *camera = new Camera(glm::radians(60.0f), (float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
int x = 0;
int y = 0;

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
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    glfwSetCursorPosCallback(window, mouse_callback);
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    camera->setPosition(glm::vec3(0.0f,0.0f,10.0f));
    
    Shader* triangleShader = new Shader();
    
    triangleShader->setShader("./Shaders/triangle.vert", GL_VERTEX_SHADER);
    triangleShader->setShader("./Shaders/triangle.frag", GL_FRAGMENT_SHADER);
    triangleShader->initialize();
    
    Mesh* triangleMesh = new Mesh();
    
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
        triangleShader->bindShader();
        triangleMesh->draw(camera);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void rotate_camera(glm::vec2 direction)
{
    glm::vec3 camPos = camera->getPosition();
    
    
    glm::vec3 cameraPos = glm::vec3(0.0f);
    
//    std::cout << glm::to_string(cameraSphericalCoords) << std::endl;
//
//    cameraPos.x = cameraSphericalCoords.x * glm::cos(cameraSphericalCoords.y) * glm::sin(cameraSphericalCoords.z);
//    cameraPos.y = cameraSphericalCoords.x * glm::sin(cameraSphericalCoords.y) * glm::sin(cameraSphericalCoords.z);
//    cameraPos.z = cameraSphericalCoords.x * glm::cos(cameraSphericalCoords.z);
//    
//    std::cout << glm::to_string(cameraPos) << std::endl;
//
//    printf("TEST:%f\n", glm::cos(cameraSphericalCoords.y));
    
//    camera.setPosition(cameraPos);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    else if(key == GLFW_KEY_UP)
        camera->setOrientation( camera->getOrientation() * glm::angleAxis(0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) );
    
    else if(key == GLFW_KEY_DOWN)
        camera->setOrientation( camera->getOrientation() * glm::angleAxis(0.5f, glm::vec3(-1.0f, 0.0f, 0.0f)) );
    
    else if(key == GLFW_KEY_RIGHT)
        camera->setOrientation( camera->getOrientation() * glm::angleAxis(0.5f, glm::vec3(0.0f, 1.0f, 0.0f)) );

    else if(key == GLFW_KEY_LEFT)
        camera->setOrientation( camera->getOrientation() * glm::angleAxis(0.5f, glm::vec3(0.0f, -1.0f, 0.0f)) );
    
    rotate_camera(glm::vec2(0.0f));
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
    
}
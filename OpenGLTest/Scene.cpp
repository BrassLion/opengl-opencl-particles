//
//  Scene.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 08/08/2016.
//
//

#include "Scene.hpp"
#include "ShaderReloader.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Viewport.hpp"
#include "Utility.hpp"

#include <GLFW/glfw3.h>

#include <OpenCL/OpenCL.h>
#include <OpenGL/OpenGL.h>

const std::string SRC_DIR = "/Users/Sam/Documents/Personal/OpenGLTest/OpenGLTest";

// Mouse variables.
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

Camera* camera;
Object* cameraContainer;

void initialize_openCL()
{
    cl_int cl_error;
    
    cl_platform_id cl_platform;
    cl_uint number_of_platforms;
    
    // Get OpenCL platform.
    cl_error = clGetPlatformIDs(1, &cl_platform, &number_of_platforms);
    
    printf("Number of platforms: %u\n", number_of_platforms);
    
    // Get OpenCL version.
    char* info_string = new char[256];
    
    cl_error = clGetPlatformInfo(cl_platform, CL_PLATFORM_VERSION, sizeof(info_string) * 128, info_string, NULL);
    
    printf("OpenCL version: %s\n", info_string);
    
    // Get GPU devices and info.
    
    cl_device_id cl_device;
    cl_uint number_of_devices;

    cl_error = clGetDeviceIDs(cl_platform, CL_DEVICE_TYPE_GPU, 1, &cl_device, &number_of_devices);
    
    printf("Number of devices: %u\n", number_of_devices);
    
    cl_error = clGetDeviceInfo(cl_device, CL_DEVICE_NAME, sizeof(info_string) * 128, info_string, NULL);
    
    printf("Device name: %s\n", info_string);
    
    // Set up OpenCL context.
    
    cl_context cl_gl_context;
    
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
    cl_context_properties props[] =
    {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
        0
    };
    
    cl_gl_context = clCreateContext(props, 0, 0, NULL, NULL, &cl_error);
    
    printf("OpenCL context creation error: %u\n", cl_error);

    // Set up OpenCL command queue.
    
    cl_command_queue cl_cmd_queue;
    
    cl_cmd_queue = clCreateCommandQueue(cl_gl_context, cl_device, 0, &cl_error);
    
    printf("OpenCL command queue creation error: %u\n", cl_error);
    
    // Set up OpenCL program.
    
    cl_program cl_prgm;
    
    std::string program_source = utility::loadFile(SRC_DIR + "/Shaders/kerneltest.cl");
    const char* program_source_char = program_source.c_str();
    size_t program_length = strlen(program_source_char);
    
    cl_prgm = clCreateProgramWithSource(cl_gl_context, 1, &program_source_char, &program_length, &cl_error);
    
    printf("OpenCL program creation error: %u\n", cl_error);
    
    // Build OpenCL program.
    
    cl_error = clBuildProgram(cl_prgm, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
    
    printf("OpenCL program build error: %u\n", cl_error);
    
    cl_error = clGetProgramBuildInfo(cl_prgm, cl_device, CL_PROGRAM_BUILD_LOG, sizeof(info_string) * 128, info_string, NULL);
    
    printf("OpenCL program build log: %s\n", info_string);
    
    // Create OpenCL kernel.
    
    cl_kernel cl_krnl;
    
    cl_krnl = clCreateKernel(cl_prgm, "texture_red", &cl_error);
    
    printf("OpenCL kernel creation error: %u\n", cl_error);

    // Create OpenGL texture and add to OpenCL.
    
    GLuint texture;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Black/white checkerboard
    float pixels[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    cl_mem texture_cl;
//    CL_INVALID_COMMAND_QUEUE
    texture_cl = clCreateFromGLTexture(cl_gl_context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, texture, &cl_error);
    
    glFinish();
    
    size_t global_work_size[] = {2, 2};
    
    cl_error = clEnqueueAcquireGLObjects(cl_cmd_queue, 1, &texture_cl, NULL, NULL, NULL);
    cl_error = clSetKernelArg(cl_krnl, 0, sizeof(texture_cl), &texture_cl);
    cl_error = clEnqueueNDRangeKernel(cl_cmd_queue, cl_krnl, 2, NULL, global_work_size, NULL, 0, 0, 0);
    cl_error = clEnqueueReleaseGLObjects(cl_cmd_queue, 1, &texture_cl, NULL, NULL, NULL);
    
    clFinish(cl_cmd_queue);
}

void Scene::initialize(int width, int height)
{
    // Camera variables.
    camera = new Camera(glm::radians(60.0f), (float)width, (float)height, 0.1f, 100.0f);
    
    // Setup viewport
    Viewport *viewport = new Viewport(0, 0, width, height);
    viewport->addCamera(camera);
    
    renderer->addViewport(viewport);
    
    cameraContainer = new Object();
    
    cameraContainer->addChild(camera);
    
    camera->setPosition(glm::vec3(0.0f,0.0f,10.0f));
    
    // Setup scene.    
    Shader* triangleShader = new Shader();
    
    triangleShader->setShader(SRC_DIR + "/Shaders/triangle.vert", GL_VERTEX_SHADER);
    triangleShader->setShader(SRC_DIR + "/Shaders/triangle.frag", GL_FRAGMENT_SHADER);
    triangleShader->initialize();
    
    Mesh* triangleMesh = new Mesh();
    
    std::vector<GLfloat> vertices = {
        // front
        -1.0, -1.0,  1.0,
        0.0, 0.0,
        
        1.0, -1.0,  1.0,
        1.0, 0.0,
        
        1.0,  1.0,  1.0,
        1.0, 1.0,
        
        -1.0,  1.0,  1.0,
        0.0, 1.0,
        
        // back
        -1.0, -1.0, -1.0,
        0.0, 0.0,
        
        1.0, -1.0, -1.0,
        1.0, 0.0,
        
        1.0,  1.0, -1.0,
        1.0, 1.0,
        
        -1.0,  1.0, -1.0,
        0.0, 1.0
    };
    std::vector<GLuint> indices = {  // Note that we start from 0!
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 0, 3,
        3, 7, 4,
        // left
        4, 5, 1,
        1, 0, 4,
        // right
        3, 2, 6,
        6, 7, 3,
    };
    
    triangleMesh->initialize(vertices, indices);
    triangleMesh->setShader(triangleShader);
    triangleMesh->setPosition( glm::vec3(0.0f,0.0f,0.0f) );
    triangleMesh->setScale( glm::vec3(1.0f,1.0f,1.0f) );
    
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
    
    initialize_openCL();
}

void Scene::draw()
{
    renderer->draw(rootNode);
}

void Scene::mouse_callback(double xpos, double ypos)
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

void Scene::scroll_callback(double xoffset, double yoffset)
{
    glm::vec3 new_camera_position = camera->getPosition() + glm::vec3(0.0f, 0.0f, yoffset);
    
    if(new_camera_position.z < 5.0f)
        new_camera_position.z = 5.0f;
    
    if(new_camera_position.z > 50.0f)
        new_camera_position.z = 50.0f;
    
    camera->setPosition( new_camera_position );
}

void Scene::key_callback(int key, int action)
{
    if(key == GLFW_KEY_UP && action != GLFW_RELEASE)
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
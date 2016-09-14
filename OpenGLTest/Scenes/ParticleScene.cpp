//
//  ParticleScene.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 21/08/2016.
//
//

#include <GL/glew.h>

#include <OpenGL/OpenGL.h>

#include "ParticleScene.hpp"
#include "Utility.hpp"
#include "Mesh.hpp"
#include "ParticleMaterial.hpp"

void ParticleScene::initialize_opencl(GLuint particleVBO)
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
        
    m_cl_cmd_queue = clCreateCommandQueue(cl_gl_context, cl_device, 0, &cl_error);
    
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
        
    m_cl_krnl = clCreateKernel(cl_prgm, "offset_test", &cl_error);
    
    printf("OpenCL kernel creation error: %u\n", cl_error);
    
    // Create OpenGL texture and add to OpenCL.
    
//    GLuint texture;
//    
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    
//    // Black/white checkerboard
//    float pixels[] = {
//        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
//        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
//    };
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
//    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    m_cl_particle_buffer = clCreateFromGLBuffer(cl_gl_context, CL_MEM_READ_WRITE, particleVBO, &cl_error);
}

void ParticleScene::run_opencl()
{
    cl_int cl_error;
    
    size_t global_work_size[] = {2, 1};

    cl_error = clEnqueueAcquireGLObjects(m_cl_cmd_queue, 1, &m_cl_particle_buffer, NULL, NULL, NULL);

    cl_error = clSetKernelArg(m_cl_krnl, 0, sizeof(m_cl_particle_buffer), &m_cl_particle_buffer);

    cl_error = clEnqueueNDRangeKernel(m_cl_cmd_queue, m_cl_krnl, 2, NULL, global_work_size, NULL, 0, 0, 0);

    cl_error = clEnqueueReleaseGLObjects(m_cl_cmd_queue, 1, &m_cl_particle_buffer, NULL, NULL, NULL);

    clFinish(m_cl_cmd_queue);
}

void ParticleScene::initialize()
{
    std::shared_ptr<Shader> particleShader( new Shader() );
    
    particleShader->setShader(SRC_DIR + "/Shaders/particle.vert", GL_VERTEX_SHADER);
    particleShader->setShader(SRC_DIR + "/Shaders/particle.frag", GL_FRAGMENT_SHADER);
    
    particleShader->initialize();
    
    std::shared_ptr<ParticleMaterial> particleMaterial( new ParticleMaterial(particleShader) );
    
    std::vector<GLfloat> vertices;
    
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    
    std::shared_ptr<Mesh> particleMesh( new Mesh() );
    
    particleMesh->initialize(vertices);
    particleMesh->setRenderingMode(GL_POINTS);
    particleMesh->setMaterial(particleMaterial);
    
    rootNode->addChild(particleMesh);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    initialize_opencl(particleMesh->getVertexBufferObject());
}

void ParticleScene::draw()
{
    Scene::draw();
    
    ParticleScene::run_opencl();
    
    glFinish();
}
//
//  ParticleScene.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 21/08/2016.
//
//

#include <GL/glew.h>

#include <OpenGL/OpenGL.h>

#include <random>

#include "ParticleScene.hpp"
#include "Utility.hpp"
#include "ParticleMaterial.hpp"
#include "VectorFieldMaterial.hpp"
#include "Texture.hpp"

void ParticleScene::initialize_opencl()
{
    cl_int cl_error;
    
    cl_platform_id cl_platform;
    cl_uint number_of_platforms;
    
    // Get OpenCL platform.
    cl_error = clGetPlatformIDs(1, &cl_platform, &number_of_platforms);
    
    CL_CHECK(cl_error);
    
    printf("Number of platforms: %u\n", number_of_platforms);
    
    // Get OpenCL version.
    char* info_string = new char[2048];
    
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
    
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
    cl_context_properties props[] =
    {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
        0
    };
    
    m_cl_gl_context = clCreateContext(props, 0, 0, NULL, NULL, &cl_error);
    
    printf("OpenCL context creation error: %u\n", cl_error);
    
    // Set up OpenCL command queue.
        
    m_cl_cmd_queue = clCreateCommandQueue(m_cl_gl_context, cl_device, 0, &cl_error);
    
    printf("OpenCL command queue creation error: %u\n", cl_error);
    
    // Set up OpenCL program.
    
    cl_program cl_prgm;
    
    std::string program_source = utility::loadFile("./Shaders/kerneltest.cl");
    const char* program_source_char = program_source.c_str();
    size_t program_length = strlen(program_source_char);
    
    cl_prgm = clCreateProgramWithSource(m_cl_gl_context, 1, &program_source_char, &program_length, &cl_error);
    
    printf("OpenCL program creation error: %u\n", cl_error);
    
    // Build OpenCL program.
    
    cl_error = clBuildProgram(cl_prgm, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
    
    cl_error = clGetProgramBuildInfo(cl_prgm, cl_device, CL_PROGRAM_BUILD_LOG, sizeof(info_string) * 2048, info_string, NULL);

    printf("OpenCL program build log: %s\n", info_string);
    
    CL_CHECK(cl_error);
    
    // Create OpenCL kernel.
        
    m_cl_krnl_particle_simulation = clCreateKernel(cl_prgm, "particle_simulation", &cl_error);
}

void ParticleScene::run_particle_simulation(float delta_time)
{
    size_t global_work_size[] = {m_current_particle_count, 1};
    cl_mem *gl_objects[] {&m_cl_particle_buffer, &m_cl_vector_field_texture};

    CL_CHECK( clEnqueueAcquireGLObjects(m_cl_cmd_queue, 2, gl_objects[0], NULL, NULL, NULL) );

    CL_CHECK( clSetKernelArg(m_cl_krnl_particle_simulation, 0, sizeof(m_cl_particle_buffer), &m_cl_particle_buffer) );
    
    CL_CHECK( clSetKernelArg(m_cl_krnl_particle_simulation, 1, sizeof(m_cl_rng_seeds), &m_cl_rng_seeds) );

    CL_CHECK( clSetKernelArg(m_cl_krnl_particle_simulation, 2, sizeof(m_cl_vector_field_texture), &m_cl_vector_field_texture) );

    CL_CHECK( clSetKernelArg(m_cl_krnl_particle_simulation, 3, sizeof(m_cl_vector_field_bounding_box), &m_cl_vector_field_bounding_box) );
    
    CL_CHECK( clSetKernelArg(m_cl_krnl_particle_simulation, 4, sizeof(float), &m_particle_tightness) );

    CL_CHECK( clSetKernelArg(m_cl_krnl_particle_simulation, 5, sizeof(float), &delta_time) );
    
    CL_CHECK( clEnqueueNDRangeKernel(m_cl_cmd_queue, m_cl_krnl_particle_simulation, 2, NULL, global_work_size, NULL, 0, 0, 0) );

    CL_CHECK( clEnqueueReleaseGLObjects(m_cl_cmd_queue, 2, gl_objects[0], NULL, NULL, NULL) );

    clFinish(m_cl_cmd_queue);
}

void ParticleScene::initialize_vector_field()
{
    std::shared_ptr<Shader> vector_field_shader(new Shader());
    
    vector_field_shader->setShader("./Shaders/vector_field.vert", GL_VERTEX_SHADER);
    vector_field_shader->setShader("./Shaders/vector_field.geom", GL_GEOMETRY_SHADER);
    vector_field_shader->setShader("./Shaders/vector_field.tesc", GL_TESS_CONTROL_SHADER);
    vector_field_shader->setShader("./Shaders/vector_field.tese", GL_TESS_EVALUATION_SHADER);
    vector_field_shader->setShader("./Shaders/vector_field.frag", GL_FRAGMENT_SHADER);
    vector_field_shader->initialize();
    
    std::shared_ptr<Texture> vector_field_texture( new Texture(2,2,2) );
    
    std::vector<GLfloat> pixels = {
        
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        
        0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0
    };
    
    vector_field_texture->initialize("./VF_Turbulence.fga");
    
    // Create OpenCL texture object.
    cl_int cl_error;
    m_cl_vector_field_texture = clCreateFromGLTexture(m_cl_gl_context, CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, vector_field_texture->get_texture_id(), &cl_error);
    CL_CHECK(cl_error);
    
    std::shared_ptr<VectorFieldMaterial> vector_field_material( new VectorFieldMaterial(vector_field_shader, vector_field_texture) );
    
    m_vector_field_mesh = std::make_shared<Mesh>();
    
    std::vector<GLfloat> vertices = {
        // front
        -1.0, -1.0,  1.0, 1.0,
        0.0, 0.0, 0.0,
        
        1.0, -1.0,  1.0, 1.0,
        1.0, 0.0, 0.0,
        
        1.0,  1.0,  1.0, 1.0,
        1.0, 1.0, 0.0,
        
        -1.0,  1.0,  1.0, 1.0,
        0.0, 1.0, 0.0,
        
        // back
        -1.0, -1.0, -1.0, 1.0,
        0.0, 0.0, 1.0,
        
        1.0, -1.0, -1.0, 1.0,
        1.0, 0.0, 1.0,
        
        1.0,  1.0, -1.0, 1.0,
        1.0, 1.0, 1.0,
        
        -1.0,  1.0, -1.0, 1.0,
        0.0, 1.0, 1.0
    };
//    std::vector<GLuint> indices = {  // Note that we start from 0!
//        // front
//        0, 1, 2,
//        2, 3, 0,
//        // top
//        1, 5, 6,
//        6, 2, 1,
//        // back
//        7, 6, 5,
//        5, 4, 7,
//        // bottom
//        4, 0, 3,
//        3, 7, 4,
//        // left
//        4, 5, 1,
//        1, 0, 4,
//        // right
//        3, 2, 6,
//        6, 7, 3,
//    };
    
    std::vector<unsigned int> attributes = {4, 3};
    
    m_vector_field_mesh->initialize(vertices, attributes);
    m_vector_field_mesh->setMaterial(vector_field_material);
    m_vector_field_mesh->setPosition( glm::vec3(0.0f,0.0f,0.0f) );
    m_vector_field_mesh->setScale( glm::vec3(2.0f,2.0f,2.0f) );
    m_vector_field_mesh->setRenderingMode(GL_PATCHES);
    m_vector_field_mesh->setNumberOfInstances(10);
    glPatchParameteri(GL_PATCH_VERTICES, 8);
    
    // Upload bounding box to OpenCL buffer.
    glm::vec4 corner1 = m_vector_field_mesh->getModelMatrix() * glm::vec4(-1.0, -1.0, 1.0, 1.0);
    glm::vec4 corner2 = m_vector_field_mesh->getModelMatrix() * glm::vec4(1.0,  1.0,  -1.0, 1.0);
    std::vector<GLfloat> bounding_box_vertices = {
        corner1.x, corner1.y, corner1.z, corner1.w,
        corner2.x, corner2.y, corner2.z, corner2.w
    };
    
    m_cl_vector_field_bounding_box = clCreateBuffer(m_cl_gl_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(GLfloat) * bounding_box_vertices.size(), bounding_box_vertices.data(), &cl_error);
    CL_CHECK(cl_error);
    
    rootNode->addChild(m_vector_field_mesh);
    
    shaderReloader->addFilesToWatch([=]{
        
        renderer->queueFunctionBeforeRender([vector_field_shader] {
            vector_field_shader->deleteShader();
            vector_field_shader->initialize();
        });
    },
                                    "./Shaders/vector_field.frag",
                                    "./Shaders/vector_field.vert",
                                    "./Shaders/vector_field.geom",
                                    "./Shaders/vector_field.tesc",
                                    "./Shaders/vector_field.tese"
                                    );
}

void ParticleScene::initialize(nanogui::Screen *gui_screen)
{
    this->initialize_opencl();
    this->initialize_vector_field();
    
    std::shared_ptr<Shader> particleShader( new Shader() );
    
    particleShader->setShader("./Shaders/particle.vert", GL_VERTEX_SHADER);
    particleShader->setShader("./Shaders/particle.frag", GL_FRAGMENT_SHADER);
    
    particleShader->initialize();
    
    std::shared_ptr<ParticleMaterial> particleMaterial( new ParticleMaterial(particleShader) );
    
    particleMesh = std::make_shared<Mesh>();
    
    particleMesh->setRenderingMode(GL_POINTS);
    particleMesh->setMaterial(particleMaterial);
    
    rootNode->addChild(particleMesh);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    this->set_particle_count(m_current_particle_count);
    
    //Shader reloading.    
    shaderReloader->addFilesToWatch([=]{
        
        renderer->queueFunctionBeforeRender([particleShader] {
            particleShader->deleteShader();
            particleShader->initialize();
        });
    },
                                    "./Shaders/particle.frag",
                                    "./Shaders/particle.vert"
                                    );
    
    this->initialize_gui(gui_screen);
}

void ParticleScene::initialize_gui(nanogui::Screen *gui_screen)
{
    // Add GUI window.
    nanogui::Window *gui_window = new nanogui::Window(gui_screen, "Particles");
    
    // Setup theme.
    nanogui::Theme *theme = gui_window->theme();
    theme->mWindowFillUnfocused = nanogui::Color(0, 0, 0, 0);
    theme->mDropShadow = nanogui::Color(0, 0, 0, 0);
    theme->mWindowHeaderGradientTop = nanogui::Color(0, 0, 0, 0);
    theme->mWindowHeaderGradientBot = nanogui::Color(0, 0, 0, 0);
    theme->mWindowHeaderSepTop = nanogui::Color(0, 0, 0, 0);
    
    gui_window->setPosition(Eigen::Vector2i(15, 110));
    gui_window->setLayout(new nanogui::GroupLayout());
    
    new nanogui::Label(gui_window, "Particle count", "sans-bold");

    nanogui::Widget *panel = new nanogui::Widget(gui_window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    nanogui::Slider *slider = new nanogui::Slider(panel);
    slider->setValue((float)m_current_particle_count / (float)m_maximum_particle_count);
    slider->setFixedWidth(80);

    nanogui::TextBox *textBox = new nanogui::TextBox(panel);
    textBox->setValue(std::to_string(m_current_particle_count));
    slider->setCallback([&, textBox](float value) {
        textBox->setValue(std::to_string((unsigned int) (m_minimum_particle_count +  value * (m_maximum_particle_count - m_minimum_particle_count))));
    });
    slider->setFinalCallback([&](float value) {
        
        unsigned int new_particle_count = (unsigned int) (m_minimum_particle_count +  value * (m_maximum_particle_count - m_minimum_particle_count));
        
        printf("Final slider value: %u\n", new_particle_count);
        
        m_current_particle_count = new_particle_count;
        
        this->set_particle_count(m_current_particle_count);
    });
    textBox->setFixedSize(Eigen::Vector2i(70,25));
    textBox->setFontSize(20);
    textBox->setAlignment(nanogui::TextBox::Alignment::Right);
    
    
    // Vector field GUI.
    
    float maximum_sample_points = 20.0f;
    
    //-------------------------------------------
    
    new nanogui::Label(gui_window, "Vector field points x", "sans-bold");

    panel = new nanogui::Widget(gui_window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    slider = new nanogui::Slider(panel);
    
    std::shared_ptr<VectorFieldMaterial> material = std::static_pointer_cast<VectorFieldMaterial>(m_vector_field_mesh->get_material());
    
    slider->setValue((float)material->get_field_sample_points_x() / maximum_sample_points);
    slider->setFixedWidth(80);
    
    textBox = new nanogui::TextBox(panel);
    textBox->setValue(std::to_string(m_current_particle_count));
    
    slider->setCallback([=](float value) {
        
        unsigned int new_sample_points_count = (unsigned int) (value * maximum_sample_points);
        
        material->set_field_sample_points_x(new_sample_points_count);
        
        textBox->setValue(std::to_string(new_sample_points_count));
    });
    
    textBox->setFixedSize(Eigen::Vector2i(70,25));
    textBox->setFontSize(20);
    textBox->setAlignment(nanogui::TextBox::Alignment::Right);
    
    //-------------------------------------------
    
    new nanogui::Label(gui_window, "Vector field points y", "sans-bold");

    panel = new nanogui::Widget(gui_window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    slider = new nanogui::Slider(panel);
    
    slider->setValue((float)material->get_field_sample_points_y() / maximum_sample_points);
    slider->setFixedWidth(80);
    
    textBox = new nanogui::TextBox(panel);
    textBox->setValue(std::to_string(m_current_particle_count));
    
    slider->setCallback([=](float value) {
        
        unsigned int new_sample_points_count = (unsigned int) (value * maximum_sample_points);
        
        material->set_field_sample_points_y(new_sample_points_count);
        
        textBox->setValue(std::to_string(new_sample_points_count));
    });
    
    textBox->setFixedSize(Eigen::Vector2i(70,25));
    textBox->setFontSize(20);
    textBox->setAlignment(nanogui::TextBox::Alignment::Right);
    
    //-------------------------------------------

    new nanogui::Label(gui_window, "Vector field points z", "sans-bold");

    panel = new nanogui::Widget(gui_window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    slider = new nanogui::Slider(panel);
    
    slider->setValue((float)m_vector_field_mesh->get_number_of_instances() / maximum_sample_points);
    slider->setFixedWidth(80);
    
    textBox = new nanogui::TextBox(panel);
    textBox->setValue(std::to_string(m_current_particle_count));
    
    slider->setCallback([=](float value) {
        
        unsigned int new_sample_points_count = (unsigned int) (value * maximum_sample_points);
        
        m_vector_field_mesh->setNumberOfInstances(new_sample_points_count);
        
        textBox->setValue(std::to_string(new_sample_points_count));
    });
    
    textBox->setFixedSize(Eigen::Vector2i(70,25));
    textBox->setFontSize(20);
    textBox->setAlignment(nanogui::TextBox::Alignment::Right);
    
    //-------------------------------------------

    new nanogui::Label(gui_window, "Particle tightness", "sans-bold");

    panel = new nanogui::Widget(gui_window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    slider = new nanogui::Slider(panel);
    
    slider->setValue((float)m_particle_tightness / 1.0f);
    slider->setFixedWidth(80);
    
    textBox = new nanogui::TextBox(panel);
    textBox->setValue(std::to_string(m_particle_tightness));
    
    slider->setCallback([=](float value) {
        
        m_particle_tightness = value;
        
        textBox->setValue(std::to_string(m_particle_tightness));
    });
    
    textBox->setFixedSize(Eigen::Vector2i(70,25));
    textBox->setFontSize(20);
    textBox->setAlignment(nanogui::TextBox::Alignment::Right);
}

void ParticleScene::set_particle_count(unsigned int particle_count)
{
    std::vector<unsigned int> particle_attributes = {4, 4, 2};
    
    unsigned int total_attributes = std::accumulate(particle_attributes.begin(), particle_attributes.end(), 0);

    std::vector<GLfloat> vertices(particle_count * total_attributes);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> position_distribution(-1, 1);
    std::uniform_real_distribution<float> velocity_distribution(0, 0);
    std::uniform_real_distribution<float> life_distribution(0, 100);
    
    for(unsigned int i = 0;i < particle_count;i++) {
        
        // Position.
        vertices[i * total_attributes] =       position_distribution(gen);
        vertices[i * total_attributes + 1] =   position_distribution(gen);
        vertices[i * total_attributes + 2] =   position_distribution(gen);
        vertices[i * total_attributes + 3] =   1.0f;
        
        // Velocity.
        vertices[i * total_attributes + 4] =   velocity_distribution(gen);
        vertices[i * total_attributes + 5] =   velocity_distribution(gen);
        vertices[i * total_attributes + 6] =   velocity_distribution(gen);
        vertices[i * total_attributes + 7] =   1.0f;
        
        // Life
        vertices[i * total_attributes + 8] =   0.0f;
        vertices[i * total_attributes + 9] =   life_distribution(gen);
    }
    
    particleMesh->initialize(vertices, particle_attributes);
    
    // Regain GL buffer in CL as it has been changed.
    cl_int cl_error;
        
    m_cl_particle_buffer = clCreateFromGLBuffer(m_cl_gl_context, CL_MEM_READ_WRITE, particleMesh->getVertexBufferObject(), &cl_error);
    
    // Generate seeds.
    
    std::vector<unsigned int> rng_seeds(particle_count * 2);
    
    std::uniform_int_distribution<unsigned int> rng_seed_distribution(0, CL_UINT_MAX);
    
    for(unsigned int i = 0;i < particle_count * 2;i++)
        rng_seeds[i] = rng_seed_distribution(gen);

    m_cl_rng_seeds = clCreateBuffer(m_cl_gl_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 2 * sizeof(unsigned int) * particle_count, rng_seeds.data(), &cl_error);
    CL_CHECK(cl_error);
}

void ParticleScene::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Scene::draw();
    
    ParticleScene::run_particle_simulation(0.0166666f);
        
    glFinish();
}
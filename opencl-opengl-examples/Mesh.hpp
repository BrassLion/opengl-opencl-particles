//
//  Mesh.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <vector>
#include <functional>

#include <GL/glew.h>

#include "Object.hpp"
#include "Material.hpp"
#include "Camera.hpp"

class Mesh : public Object
{
private:
    GLuint VBO, VAO, EBO;
    
    GLenum m_rendering_mode;
    
    unsigned int m_number_of_vertices;
    unsigned int m_number_of_indices;
    unsigned int m_number_of_instances;
    
    std::shared_ptr<Material> m_material;
    
    std::function<void()> m_draw;
    
    void set_drawing_function();

public:
    
    Mesh() {
        m_rendering_mode = GL_TRIANGLES;
                
        EBO = 0;
        m_number_of_indices = 0;
        m_number_of_instances = 0;
    }
    
    ~Mesh() {
        deleteMesh();
    }
    
    void initialize(std::vector<GLfloat> vertices, std::vector<unsigned int> attributes, std::vector<GLuint> indices);
    void initialize(std::vector<GLfloat> vertices, std::vector<unsigned int> attributes);
    void setMaterial(std::shared_ptr<Material> material);
    void setRenderingMode(GLenum rendering_mode);
    void setNumberOfInstances(unsigned int number_of_instances);
    
    GLuint getVertexBufferObject();
    std::shared_ptr<Material> get_material();
    unsigned int get_number_of_instances();
    
    void draw(std::shared_ptr<Camera> camera);
    
    void deleteMesh();
};

#endif /* Mesh_hpp */

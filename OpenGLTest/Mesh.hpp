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
    
    std::shared_ptr<Material> m_material;

public:
    
    Mesh() {
        m_rendering_mode = GL_TRIANGLES;
        
        EBO = 0;
    }
    
    ~Mesh() {
        deleteMesh();
    }
    
    void initialize(std::vector<GLfloat> vertices, std::vector<unsigned int> attributes, std::vector<GLuint> indices);
    void initialize(std::vector<GLfloat> vertices, std::vector<unsigned int> attributes);
    void setMaterial(std::shared_ptr<Material> material);
    void setRenderingMode(GLenum rendering_mode);
    GLuint getVertexBufferObject();
    void draw(std::shared_ptr<Camera> camera);
    
    void deleteMesh();
};

#endif /* Mesh_hpp */

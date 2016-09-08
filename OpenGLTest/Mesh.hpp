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
#include "Shader.hpp"
#include "Camera.hpp"

class Mesh : public Object
{
private:
    GLuint VBO, VAO, EBO;
    
    int m_number_of_vertices;
    
    std::shared_ptr<Shader> meshShader;

public:
    
    ~Mesh() {
        deleteMesh();
    }
    
    void initialize(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
    void setShader(std::shared_ptr<Shader> shader);
    void draw(std::shared_ptr<Camera> camera);
    
    void deleteMesh();
};

#endif /* Mesh_hpp */

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

class Mesh : public Object
{
private:
    GLuint VBO, VAO, EBO;
    Shader meshShader;

public:
    
    void initialize(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
    void setShader(Shader shader);
    void draw();
};

#endif /* Mesh_hpp */

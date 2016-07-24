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

class Mesh
{
private:
    GLuint VBO, VAO, EBO;

public:
    
    void initialize(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
    void draw();
};

#endif /* Mesh_hpp */

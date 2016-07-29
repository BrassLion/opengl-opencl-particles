//
//  Mesh.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#include "Mesh.hpp"

void Mesh::initialize(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

void Mesh::setShader(Shader* shader)
{
    meshShader = shader;
}

void Mesh::draw(Camera* camera)
{
    meshShader->bindShader();
    meshShader->setUniform("mvpMatrix", camera->getProjectionMatrix() * camera->getViewMatrix()  * modelMatrix);
    
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
    Object::draw(camera);
}
//
//  Mesh.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#include "Mesh.hpp"

#include <glm/gtx/string_cast.hpp>

void Mesh::initialize(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
{
    m_number_of_vertices = (int) indices.size();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
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
    meshShader->setUniform("model_matrix", modelMatrix);
    meshShader->setUniform("camera_world_position", camera->getModelMatrix() * glm::vec4(camera->getPosition(), 0.0) );
    
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, m_number_of_vertices, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
    Object::draw(camera);
}
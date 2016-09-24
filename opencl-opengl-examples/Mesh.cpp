//
//  Mesh.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#include "Mesh.hpp"

#include <glm/gtx/string_cast.hpp>

void Mesh::initialize(std::vector<GLfloat> vertices, std::vector<unsigned int> attributes, std::vector<GLuint> indices)
{
    initialize(vertices, attributes);
    
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
    m_number_of_indices = (unsigned int)indices.size();
    set_drawing_function();
}

void Mesh::initialize(std::vector<GLfloat> vertices, std::vector<unsigned int> attributes)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    // Assign vertex attributes.
    unsigned int total_number_of_attributes = 0;
    unsigned int current_attribute_offset = 0;
    
    std::for_each(attributes.begin(), attributes.end(), [&] (unsigned int n) { total_number_of_attributes += n; } );
    
    for (unsigned int i = 0; i < attributes.size(); i++) {
        
        glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, total_number_of_attributes * sizeof(GLfloat), (GLvoid*)(current_attribute_offset * sizeof(GLfloat)));
        glEnableVertexAttribArray(i);
        
        current_attribute_offset += attributes[i];
    }
    // Position.
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    m_number_of_vertices = (unsigned int)vertices.size() / total_number_of_attributes;
    set_drawing_function();
}

void Mesh::setMaterial(std::shared_ptr<Material> material)
{
    m_material = material;
    
    material->set_mesh(std::static_pointer_cast<Mesh>(shared_from_this()));			
}

void Mesh::setRenderingMode(GLenum rendering_mode)
{
    m_rendering_mode = rendering_mode;
    
    set_drawing_function();
}

void Mesh::setNumberOfInstances(unsigned int number_of_instances)
{
    m_number_of_instances = number_of_instances;
    
    set_drawing_function();
}

void Mesh::set_drawing_function()
{
    if (m_rendering_mode == GL_POINTS || m_number_of_indices == 0) {
        
        if (m_number_of_instances > 0)
            m_draw = [=] { glDrawArraysInstanced(m_rendering_mode, 0, m_number_of_vertices, m_number_of_instances); };
        
        else
            m_draw = [=] { glDrawArrays(m_rendering_mode, 0, m_number_of_vertices); };
    }

    else if (m_number_of_indices > 0)
        m_draw = [=] { glDrawElements( m_rendering_mode, m_number_of_indices, GL_UNSIGNED_INT, 0); };
}

GLuint Mesh::getVertexBufferObject()
{
    return VBO;
}

std::shared_ptr<Material> Mesh::get_material()
{
    return m_material;
}

unsigned int Mesh::get_number_of_instances()
{
    return m_number_of_instances;
}

void Mesh::draw(std::shared_ptr<Camera> camera)
{
    m_material->apply(shared_from_this(), camera);
    
    glBindVertexArray(VAO);
    
    m_draw();
    
    glBindVertexArray(0);
    
    Object::draw(camera);
}

void Mesh::deleteMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

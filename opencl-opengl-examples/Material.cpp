
//
//  Material.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 11/09/2016.
//
//

#include "Material.hpp"

void Material::set_mesh(std::shared_ptr<Mesh> mesh)
{
    m_mesh = mesh;
}

void Material::apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera)
{
    m_shader->bindShader();
    
    m_shader->setUniform("mvpMatrix", camera->getProjectionMatrix() * camera->getViewMatrix() * object->getModelMatrix());
    m_shader->setUniform("model_matrix", object->getModelMatrix());
}
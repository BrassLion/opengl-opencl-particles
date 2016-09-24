//
//  VectorFieldMaterial.cpp
//  opencl-opengl-examples
//
//  Created by Samuel Hall on 19/09/2016.
//
//

#include "VectorFieldMaterial.hpp"
#include "Mesh.hpp"

void VectorFieldMaterial::set_field_sample_points_x(unsigned int sample_points)
{
    m_sample_points_x = sample_points;
}

void VectorFieldMaterial::set_field_sample_points_y(unsigned int sample_points)
{
    m_sample_points_y = sample_points;
}

unsigned int VectorFieldMaterial::get_field_sample_points_x()
{
    return m_sample_points_x;
}

unsigned int VectorFieldMaterial::get_field_sample_points_y()
{
    return m_sample_points_y;
}

void VectorFieldMaterial::apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera)
{
    Material::apply(object, camera);
    
    m_vector_field_texture->bind_texture();
        
    m_shader->setUniform("field_sample_points_x", m_sample_points_x);
    m_shader->setUniform("field_sample_points_y", m_sample_points_y);
    m_shader->setUniform("field_sample_points_z", m_mesh.lock()->get_number_of_instances() - 1);
}
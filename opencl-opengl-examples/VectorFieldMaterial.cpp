//
//  VectorFieldMaterial.cpp
//  opencl-opengl-examples
//
//  Created by Samuel Hall on 19/09/2016.
//
//

#include "VectorFieldMaterial.hpp"

void VectorFieldMaterial::apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera)
{
    Material::apply(object, camera);
    
    m_vector_field_texture->bind_texture();
}
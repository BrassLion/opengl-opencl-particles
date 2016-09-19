//
//  VectorFieldMaterial.hpp
//  opencl-opengl-examples
//
//  Created by Samuel Hall on 19/09/2016.
//
//

#ifndef VectorFieldMaterial_hpp
#define VectorFieldMaterial_hpp

#include <stdio.h>

#include "Material.hpp"
#include "Texture.hpp"

class VectorFieldMaterial : public Material
{
private:
    
    std::shared_ptr<Texture> m_vector_field_texture;
    
public:
    
    VectorFieldMaterial(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> vector_field_texture) : Material(shader)
    {
        m_vector_field_texture = vector_field_texture;
    }
    
    void apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera);
};

#endif /* VectorFieldMaterial_hpp */

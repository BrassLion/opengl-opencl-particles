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
    
    unsigned int m_sample_points_x;
    unsigned int m_sample_points_y;
    
public:
    
    VectorFieldMaterial(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> vector_field_texture) : Material(shader)
    {
        m_vector_field_texture = vector_field_texture;
        m_sample_points_x = 2;
        m_sample_points_y = 2;
    }
    
    void set_field_sample_points_x(unsigned int sample_points);
    void set_field_sample_points_y(unsigned int sample_points);
    
    unsigned int get_field_sample_points_x();
    unsigned int get_field_sample_points_y();
    
    void apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera);
};

#endif /* VectorFieldMaterial_hpp */

//
//  Material.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 11/09/2016.
//
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>

#include "Shader.hpp"
#include "Object.hpp"
#include "Camera.hpp"

class Material
{
private:
    std::shared_ptr<Shader> m_shader;
    
public:
    Material(std::shared_ptr<Shader> shader)
    {
        m_shader = shader;
    }
    
    virtual void apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera);
};

#endif /* Material_hpp */

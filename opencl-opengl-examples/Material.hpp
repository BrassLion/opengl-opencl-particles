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

class Mesh;

class Material
{
private:
    
protected:
    std::shared_ptr<Shader> m_shader;
    std::weak_ptr<Mesh> m_mesh;
    
public:
    Material(std::shared_ptr<Shader> shader)
    {
        m_shader = shader;
    }
    
    void set_mesh(std::shared_ptr<Mesh> mesh);
    
    virtual void apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera);
};

#endif /* Material_hpp */

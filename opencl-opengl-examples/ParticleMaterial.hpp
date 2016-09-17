//
//  ParticleMaterial.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 12/09/2016.
//
//

#ifndef ParticleMaterial_hpp
#define ParticleMaterial_hpp

#include <stdio.h>

#include "Material.hpp"

class ParticleMaterial : public Material
{
public:
    using Material::Material;
    
    void apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera); 
};

#endif /* ParticleMaterial_hpp */

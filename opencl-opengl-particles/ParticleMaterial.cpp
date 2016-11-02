//
//  ParticleMaterial.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 12/09/2016.
//
//

#include "ParticleMaterial.hpp"

void ParticleMaterial::apply(std::shared_ptr<Object> object, std::shared_ptr<Camera> camera)
{
    Material::apply(object, camera);
}
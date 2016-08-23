//
//  ParticleScene.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 21/08/2016.
//
//

#ifndef ParticleScene_hpp
#define ParticleScene_hpp

#include <stdio.h>

#include "Scene.hpp"

class ParticleScene : public Scene
{
    using Scene::Scene;
    
private:
    
    void initialize_opencl();
    
public:
    
    void initialize();
};

#endif /* ParticleScene_hpp */

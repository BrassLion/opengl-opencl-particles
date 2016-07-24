//
//  Object.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Object
{
protected:
    
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale = glm::vec3(1.0f);;
    
    glm::mat4 modelMatrix;
    
    void updateModelMatrix();
    
public:
    
    void setPosition(glm::vec3 newPosition);
    void setOrientation(glm::quat newOrientation);
    void setScale(glm::vec3 newScale);
};

#endif /* Object_hpp */

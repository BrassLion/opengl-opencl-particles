//
//  Camera.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.hpp"

class Camera : public Object
{
private:
    glm::mat4 projection;
    
public:
    
    Camera(float fov, float width, float height, float near, float far)
    {
        projection = glm::perspectiveFov(fov, width, height, near, far);
    }
};

#endif /* Camera_hpp */

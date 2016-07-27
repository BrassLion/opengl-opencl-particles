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
    glm::mat4 view;
    
    glm::vec3 target = glm::vec3(0.0f);
    glm::vec3 up = glm::normalize( glm::vec3(0.0f, 0.5f, 0.5f) );
    
protected:
    
    void updateModelMatrix();
    
public:
    
    Camera(float fov, float width, float height, float near, float far)
    {
        projection = glm::perspectiveFov(fov, width, height, near, far);
    }
    
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();    
};

#endif /* Camera_hpp */

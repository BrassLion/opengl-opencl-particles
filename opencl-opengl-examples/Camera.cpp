//
//  Camera.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#include <iostream>

#include "Camera.hpp"

glm::mat4 Camera::getProjectionMatrix()
{
    return projection;
}

glm::mat4 Camera::getViewMatrix()
{
    return view;
}

void Camera::updateModelMatrix()
{
    Object::updateModelMatrix();
    
    view = glm::inverse(modelMatrix);
}
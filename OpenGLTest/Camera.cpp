//
//  Camera.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#include "Camera.hpp"

glm::mat4 Camera::getProjectionMatrix()
{
    return projection;
}

glm::mat4 Camera::getViewMatrix()
{
    return view;
}

void Camera::updateViewMatrix()
{
    view = glm::lookAt(position, target, up);
}

void Camera::setPosition(glm::vec3 newPosition)
{
    Object::setPosition(newPosition);
    
    updateViewMatrix();
}
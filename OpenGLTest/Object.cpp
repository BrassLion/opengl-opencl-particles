//
//  Object.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#include "Object.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void Object::setPosition(glm::vec3 newPosition)
{
    position = newPosition;
    
    updateModelMatrix();
}

void Object::setOrientation(glm::quat newOrientation)
{
    orientation = newOrientation;
    
    updateModelMatrix();
}

void Object::setScale(glm::vec3 newScale)
{
    scale = newScale;
    
    updateModelMatrix();
}

void Object::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::mat4_cast(orientation) * modelMatrix;
    modelMatrix = glm::scale(modelMatrix, scale);
}
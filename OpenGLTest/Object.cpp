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
#include <iostream>

void Object::setPosition(glm::vec3 newPosition)
{
    position = newPosition;
    
    updateModelMatrix();
}

glm::vec3 Object::getPosition()
{
    return position;
}

void Object::setOrientation(glm::quat newOrientation)
{
    orientation = newOrientation;
        
    updateModelMatrix();
}

glm::quat Object::getOrientation()
{
    return orientation;
}

void Object::setScale(glm::vec3 newScale)
{
    scale = newScale;
    
    updateModelMatrix();
}

glm::mat4 Object::getModelMatrix()
{
    return modelMatrix;
}

void Object::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = modelMatrix * glm::mat4_cast(orientation);
    modelMatrix = glm::scale(modelMatrix, scale);
    
    if(parent)
    {
        modelMatrix = parent->getModelMatrix() * modelMatrix;
    }
    
    for(Object *child : children)
        child->updateModelMatrix();
}

void Object::setParent(Object *newParent)
{
    parent = newParent;
}

void Object::addChild(Object *child)
{
    children.push_back(child);
    
    child->setParent(this);
    
    child->updateModelMatrix();
}
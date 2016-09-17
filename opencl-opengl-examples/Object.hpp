//
//  Object.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 24/07/2016.
//
//

#ifndef Object_hpp
#define Object_hpp

#include <vector>

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera;

class Object : public std::enable_shared_from_this<Object>
{
protected:
    
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale = glm::vec3(1.0f);;
    
    glm::mat4 modelMatrix;
    
    std::weak_ptr<Object> parent;
    std::vector<std::shared_ptr<Object>> m_children;
    
    virtual void updateModelMatrix();
    
public:
    
    void setPosition(glm::vec3 newPosition);
    glm::vec3 getPosition();
    
    void setOrientation(glm::quat newOrientation);
    glm::quat getOrientation();
    
    void setScale(glm::vec3 newScale);
    
    glm::mat4 getModelMatrix();
    
    void addChild(std::shared_ptr<Object> child);
    
    void setParent(std::shared_ptr<Object> parent);
    
    virtual void draw(std::shared_ptr<Camera> camera);
};

#endif /* Object_hpp */

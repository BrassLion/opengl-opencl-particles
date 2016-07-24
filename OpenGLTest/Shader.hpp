//
//  Shader.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 23/07/2016.
//
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>

class Shader
{
private:
    
    GLuint programID;

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    
    GLint getUniformLocation(std::string uniform);
    
public:
    
    bool initialize();
    void setShader(std::string shaderPath,  GLuint shaderType);
    void bindShader();
    
    void setUniform(std::string uniform, glm::mat4 value);
};

#endif /* Shader_hpp */
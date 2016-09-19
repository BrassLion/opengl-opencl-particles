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
    std::string geometryShaderPath;
    std::string fragmentShaderPath;
    
    GLint getUniformLocation(std::string uniform);
    
public:
    
    ~Shader() {
        deleteShader();
    }
    
    bool initialize();
    void deleteShader();
    void setShader(std::string shaderPath,  GLuint shaderType);
    void bindShader();
    
    void setUniform(std::string uniform, glm::mat4 value);
    void setUniform(std::string uniform, glm::vec3 value);
    void setUniform(std::string uniform, glm::vec4 value);
};

#endif /* Shader_hpp */

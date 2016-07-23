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

class Shader
{
private:

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    
public:
    
    bool initialize();
    void setShader(std::string shaderPath,  GLuint shaderType);
};

#endif /* Shader_hpp */

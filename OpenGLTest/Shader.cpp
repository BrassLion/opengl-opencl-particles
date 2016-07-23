//
//  Shader.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 23/07/2016.
//
//

#include "Shader.hpp"

#include <stdlib.h>
#include <stdexcept>
#include <assert.h>
#include <fstream>

/// Utility functions.
std::string readShaderFile(std::string shaderPath)
{
    std::ifstream ifs(shaderPath);
    
    if (!ifs.is_open()) {
        
        throw std::invalid_argument( "File cannot be opened." );
    }
    
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>()    ) );
        
    return content;
}

/// Public functions.
void Shader::setShader(std::string shaderPath, GLuint shaderType)
{
    switch (shaderType) {
            
        case GL_VERTEX_SHADER:
            vertexShaderPath = shaderPath;
            break;
            
        case GL_FRAGMENT_SHADER:
            fragmentShaderPath = shaderPath;
            break;
            
        default:
            throw std::invalid_argument( "Shader type not supported." );
            break;
    }
}

bool Shader::initialize()
{
    if (vertexShaderPath.empty()) {
        
        throw std::invalid_argument( "Vertex shader undefined." );
        return false;
    }
    
    if(fragmentShaderPath.empty()) {
        
        throw std::invalid_argument( "Fragment shader undefined. ");
        return false;
    }
    
    printf("%s", readShaderFile(vertexShaderPath).c_str());
    
    return true;
}
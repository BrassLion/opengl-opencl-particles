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
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "Utility.hpp"

bool checkShaderError(GLuint shader)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw std::runtime_error(std::string("Shader compilation error:\n") + infoLog);
        
        return false;
    }
    
    return true;
}

bool checkProgramError(GLuint program)
{
    GLint success;
    GLchar infoLog[512];
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        throw std::runtime_error(std::string("Program link error:\n") + infoLog);
        
        return false;
    }
    
    return true;
}

GLuint compileShader(std::string shaderPath, GLuint shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    
    std::string shaderSourceString = utility::loadFile(shaderPath);
    const char *shaderSource = shaderSourceString.c_str();
    
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    
    checkShaderError(shader);
    
    return shader;
}

/// Public functions.
void Shader::setShader(std::string shaderPath, GLuint shaderType)
{
    switch (shaderType) {
            
        case GL_VERTEX_SHADER:
            vertexShaderPath = shaderPath;
            break;
        
        case GL_GEOMETRY_SHADER:
            geometryShaderPath = shaderPath;
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
        
        throw std::runtime_error( "Vertex shader undefined." );
        return false;
    }
    
    if(fragmentShaderPath.empty()) {
        
        throw std::runtime_error( "Fragment shader undefined. ");
        return false;
    }
    
    // Build and compile shaders.
    std::vector<GLuint> shaders;
    
    shaders.push_back(compileShader(vertexShaderPath, GL_VERTEX_SHADER));
    shaders.push_back(compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER));
    
    if(!geometryShaderPath.empty())
        shaders.push_back(compileShader(geometryShaderPath, GL_GEOMETRY_SHADER));
    
    // Build program.
    programID = glCreateProgram();
    
    for(GLuint shader : shaders)
        glAttachShader(programID, shader);
    
    glLinkProgram(programID);
    
    // Check for linking errors.
    checkProgramError(programID);
    
    // Delete shaders.
    for(GLuint shader: shaders)
        glDeleteShader(shader);
    
    return true;
}

void Shader::deleteShader()
{
    glDeleteShader(programID);
}

void Shader::bindShader()
{
    glUseProgram(programID);
}

GLint Shader::getUniformLocation(std::string uniform)
{
    GLint uniformLocation = glGetUniformLocation(programID, uniform.c_str());
    
    if(uniformLocation == -1)
        throw std::invalid_argument( "Uniform \"" + uniform + "\" does not exist." );
    
    return uniformLocation;
}

void Shader::setUniform(std::string uniform, glm::mat4 value)
{
    GLint uniformLocation = getUniformLocation(uniform);
    
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(std::string uniform, glm::vec3 value)
{
    GLint uniformLocation = getUniformLocation(uniform);
    
    glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
}

void Shader::setUniform(std::string uniform, glm::vec4 value)
{
    GLint uniformLocation = getUniformLocation(uniform);
    
    glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
}


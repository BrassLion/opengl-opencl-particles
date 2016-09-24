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
    ShaderFile shader_file;
    
    shader_file.path = shaderPath;
    shader_file.type = shaderType;
    
    m_shader_files.push_back(shader_file);
}

bool Shader::initialize()
{
    // Build and compile shaders.
    std::vector<GLuint> shaders;
    
    for(ShaderFile shader_file : m_shader_files)
        shaders.push_back(compileShader(shader_file.path, shader_file.type));
    
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

void Shader::setUniform(std::string uniform, unsigned int value)
{
    GLint uniformLocation = getUniformLocation(uniform);
    
    glUniform1i(uniformLocation, value);
}


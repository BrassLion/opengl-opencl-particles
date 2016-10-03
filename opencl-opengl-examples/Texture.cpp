//
//  Texture3D.cpp
//  opencl-opengl-examples
//
//  Created by Samuel Hall on 19/09/2016.
//
//

#include "Texture.hpp"
#include "Utility.hpp"

#include <cmath>

void Texture::initialize(std::vector<GLfloat> pixels)
{
    glGenTextures(1, &m_texture_id);
    
    this->bind_texture();
    
    unsigned int number_of_pixels = m_width * m_height * m_depth;
    unsigned long number_of_components = pixels.size() / number_of_pixels;
    
    GLenum texture_format;
    
    switch (number_of_components) {
        case 1:
            texture_format = GL_RED;
        break;
        
        case 2:
            texture_format = GL_RG;
        break;
        
        case 3:
            texture_format = GL_RGB;
        break;
        
        case 4:
            texture_format = GL_RGBA;
        break;
        
        default:
            throw std::invalid_argument( "Invalid texture format." );
        break;
    }
    
    if(m_texture_type == GL_TEXTURE_2D)
        glTexImage2D(GL_TEXTURE_2D, 0, texture_format, m_width, m_height, 0, texture_format, GL_FLOAT, pixels.data());
    
    else if(m_texture_type == GL_TEXTURE_3D)
        glTexImage3D(GL_TEXTURE_3D, 0, texture_format, m_width, m_height, m_depth, 0, texture_format, GL_FLOAT, pixels.data());
    
    
    glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(m_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::initialize(std::string file_path)
{
    std::string file_extension = file_path.substr(file_path.find_last_of('.'), file_path.size());
    
    if (file_extension != ".fga") {
        
        throw std::invalid_argument("Unsupported texture file type \"" + file_extension + "\"");
    }
    
    std::vector<GLfloat> pixels = utility::load_file_to_values<GLfloat>(file_path);
    
    int size = cbrt(pixels.size() / 3);
    
    m_width = size;
    m_height = size;
    m_depth = size;
    
    this->initialize(pixels);
}

void Texture::bind_texture()
{
    glBindTexture(m_texture_type, m_texture_id);
}

GLuint Texture::get_texture_id()
{
    return m_texture_id;
}


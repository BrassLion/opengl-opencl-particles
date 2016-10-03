//
//  Texture3D.hpp
//  opencl-opengl-examples
//
//  Created by Samuel Hall on 19/09/2016.
//
//

#ifndef Texture3D_hpp
#define Texture3D_hpp

#include <stdlib.h>
#include <stdexcept>
#include <vector>

#include <GL/glew.h>

class Texture
{
private:
    
    GLuint m_texture_id;
    GLenum m_texture_type;
    
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_depth;
    
public:
    Texture(unsigned int width, unsigned int height, unsigned int depth)
    {
        if(width == 0 || height == 0 || depth == 0)
            throw std::invalid_argument( "Texture cannot have a length of 0." );

        m_width = width;
        m_height = height;
        m_depth = depth;
        
        if(depth > 1)
            m_texture_type = GL_TEXTURE_3D;
        
        else if(height > 1)
            m_texture_type = GL_TEXTURE_2D;
        
        else
            throw std::invalid_argument( "Texture type not initialized." );
    }
    
    Texture(unsigned int width, unsigned int height)
    {
        Texture(width, height, 1);
    }
    
    ~Texture()
    {
        glDeleteTextures(1, &m_texture_id);
    }
    
    void initialize(std::vector<GLfloat> pixels);
    void initialize(std::string file_path);
    void bind_texture();
    
    GLuint get_texture_id();
};

#endif /* Texture3D_hpp */

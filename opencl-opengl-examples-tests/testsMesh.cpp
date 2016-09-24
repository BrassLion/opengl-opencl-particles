//
//  testsMesh.cpp
//  opencl-opengl-examples
//
//  Created by Samuel Hall on 20/09/2016.
//
//

#include <stdio.h>
#include <GL/glew.h>

#include "catch.hh"

#include "Mesh.hpp"

TEST_CASE( "Mesh data loads", "[mesh]" ) {
    
    std::shared_ptr<Mesh> mesh( new Mesh() );
    
    std::vector<GLfloat> vertices = {
        
        1.0f, 2.0f, 3.0f,
        6.8f, 1.2f, 4.5f,
        5.7f, 11.5f, 23.0f
    };
    
    std::vector<GLuint> attributes = {
        3
    };
    
    mesh->initialize(vertices, attributes);
    
    
}
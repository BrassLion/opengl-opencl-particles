//
//  Utility.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 10/08/2016.
//
//

#include "Utility.hpp"

namespace utility
{    
    std::string loadFile(std::string filePath)
    {
        std::ifstream ifs(filePath);
        
        if (!ifs.is_open()) {
            
            throw std::invalid_argument( "File cannot be opened." );
        }
        
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()    ) );
        
        return content;
    }
}
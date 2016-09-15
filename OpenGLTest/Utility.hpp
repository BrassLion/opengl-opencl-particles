//
//  Utility.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 10/08/2016.
//
//

#ifndef Utility_hpp
#define Utility_hpp

#include <stdio.h>
#include <stdlib.h>

#include <fstream>


namespace utility
{    
    std::string loadFile(std::string filePath);
    
    class profiler
    {
    private:
        std::chrono::high_resolution_clock m_clock;
        std::chrono::high_resolution_clock::time_point m_start;
        std::chrono::high_resolution_clock::time_point m_split;
        
    public:
        void start();
        void split();
        void stop();
    };
}

#endif /* Utility_hpp */

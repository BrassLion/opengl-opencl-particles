//
//  Utility.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 10/08/2016.
//
//

#include "Utility.hpp"
#include <unistd.h>
namespace utility
{    
    std::string loadFile(std::string filePath)
    {
        std::ifstream ifs(filePath);
                
        if (!ifs.is_open()) {
            
            char * dir = getcwd(NULL, 0); // Platform-dependent, see reference link below
            printf("Current dir: %s\n", dir);
            
            throw std::invalid_argument( "File \"%s\" cannot be opened." );
        }
        
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()    ) );
        
        return content;
    }
    
    void profiler::start()
    {
        m_start = m_clock.now();
        m_split = m_start;
    }
    
    void profiler::split()
    {
        std::chrono::high_resolution_clock::time_point new_split = m_clock.now();
        
        printf("Time elapsed: %lldus\n", std::chrono::duration_cast<std::chrono::microseconds>(new_split - m_split).count());
        
        m_split = new_split;        
    }
    
    void profiler::stop()
    {
        m_split = m_clock.now();
        
        printf("Total time elapsed: %lldus\n", std::chrono::duration_cast<std::chrono::microseconds>(m_split - m_start).count());
    }
}
//
//  Utility.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 10/08/2016.
//
//

#ifndef Utility_hpp
#define Utility_hpp

#include <OpenCL/OpenCL.h>

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <fstream>
#include <sstream>

#ifdef DEBUG
#define CL_CHECK(error) \
utility::check_opencl_error(error, __FILE__, __LINE__);
#else
#define CL_CHECK(error)
#endif

namespace utility
{
    std::ifstream get_file_stream(std::string file_path);
    std::string loadFile(std::string filePath);
    
    template<typename T>
    std::vector<T> load_file_to_values(std::string file_path)
    {
        std::ifstream ifs = get_file_stream(file_path);
        std::vector<T> result;
        
        std::string file_line;
        
        while ( std::getline(ifs, file_line, ',') ) {
            
            std::istringstream is(file_line);
            
            T value;
            
            if ( is >> value )
                result.push_back(value);
        }
        
        return result;
    }
    
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
    
    const char *get_opencl_error_string(cl_int error);
    void check_opencl_error(cl_int error, const char* fname, int line);
}

#endif /* Utility_hpp */

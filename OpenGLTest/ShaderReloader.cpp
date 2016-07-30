//
//  ShaderReloader.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 27/07/2016.
//
//

#include "ShaderReloader.hpp"

#include <iostream>

void ShaderReloader::initialize()
{
    is_running = true;
    
    monitoring_thread = std::thread( [this] {
        
        while (is_running) {
            
            for (File &file : watched_files) {
                
                std::time_t last_write_time = boost::filesystem::last_write_time(file.path);
                
                if(last_write_time > file.last_write_time) {
                    
                    file.last_write_time = last_write_time;
                    
                    file.callback();
                }
            }
            
            std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
        }
    });
}

void ShaderReloader::deinitialize()
{
    is_running = false;
    
    monitoring_thread.detach();
}

void ShaderReloader::addFilesToWatch(std::function<void ()> callback)
{
}

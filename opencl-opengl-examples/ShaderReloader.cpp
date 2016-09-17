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
    printf("init\n");

    is_running = true;
    deinit_mutex.lock();
    
    monitoring_thread = std::thread( [&] {
                
        while (is_running) {
                        
            deinit_mutex.try_lock_for(std::chrono::milliseconds(1000));

            for (std::shared_ptr<File> file : watched_files) {
                                
                std::time_t last_write_time = boost::filesystem::last_write_time(file->path);
                
                if(last_write_time > file->last_write_time) {
                    
                    file->last_write_time = last_write_time;
                    
                    file->callback();
                }
            }
        }
    });
}

void ShaderReloader::deinitialize()
{
    is_running = false;
    
    deinit_mutex.unlock();
    
    monitoring_thread.join();
}

void ShaderReloader::addFilesToWatch(std::function<void ()> callback)
{
}
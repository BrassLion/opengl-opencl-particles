//
//  ShaderReloader.hpp
//  OpenGLTest
//
//  Created by Samuel Hall on 27/07/2016.
//
//

#ifndef ShaderReloader_hpp
#define ShaderReloader_hpp

#include <stdio.h>

#include <vector>
#include <thread>
#include <ctime>
#include <boost/filesystem.hpp>

class ShaderReloader
{
private:
    
    ShaderReloader() {
        
        initialize();
    };
    
    ~ShaderReloader() {
    
        deinitialize();
    };
    
    std::thread monitoring_thread;
    bool is_running;
    
    struct File {
        boost::filesystem::path path;
        std::time_t last_write_time;
        std::function<void()> callback;
    };
    
    std::vector<File> watched_files;
    
    void initialize();
    void deinitialize();
    
public:
    
    static ShaderReloader& getInstance()
    {
        static ShaderReloader instance;
        
        return instance;
    }
    
    ShaderReloader(ShaderReloader const&)  = delete;
    void operator=(ShaderReloader const&)  = delete;
    
    void addFileToWatch(std::string filePath, std::function<void()> callback);
};

#endif /* ShaderReloader_hpp */

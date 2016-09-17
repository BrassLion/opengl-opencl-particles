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
    
    std::thread monitoring_thread;
    bool is_running;
    std::condition_variable thread_conditional;
    std::mutex thread_mutex;
    std::timed_mutex deinit_mutex;
    
    struct File {
        boost::filesystem::path path;
        std::time_t last_write_time;
        std::function<void()> callback;
    };
    
    std::vector<std::shared_ptr<File>> watched_files;
    
    void initialize();
    void deinitialize();
    
    void pollFiles();
    
    void addFilesToWatch(std::function<void ()> callback);

public:
    
    ShaderReloader() {
        
        initialize();
    };
    
    ~ShaderReloader() {
        
        deinitialize();
    };
        
    template<typename ... Args>
	    void addFilesToWatch(std::function<void ()> callback, std::string filePath, Args ... args);
};

template<typename ... Args>
inline void ShaderReloader::addFilesToWatch(std::function<void ()> callback, std::string filePath, Args ... args)
{
    std::shared_ptr<File> file(new File());
    
    file->path = boost::filesystem::path(filePath);
    file->last_write_time = boost::filesystem::last_write_time(file->path);
    file->callback = callback;
    
    watched_files.push_back(file);
    
    addFilesToWatch(callback, args...);
}


#endif /* ShaderReloader_hpp */

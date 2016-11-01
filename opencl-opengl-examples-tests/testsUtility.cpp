//
//  testsUtility.cpp
//  OpenGLTest
//
//  Created by Samuel Hall on 16/09/2016.
//
//

#include "catch.hh"

#include "Utility.hpp"

#include <thread>

TEST_CASE( "profiler starts and stops", "[utility]" )
{
    std::unique_ptr<utility::profiler> m_profiler( new utility::profiler() );
    
    m_profiler->start();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(11));
    
    m_profiler->split();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(9));
    
    m_profiler->stop();
}

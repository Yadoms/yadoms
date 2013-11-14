# Project Stuff
cmake_minimum_required (VERSION 2.6)
project (Yadoms)

#incluse user specific configuration
include("CMakeListsUserConfig.txt" OPTIONAL)

# Default Options
if(UNIX AND NOT APPLE)
	add_definitions("-std=c++0x")
endif()

#suppress VS solution ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Find Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_LIBS thread date_time system chrono log filesystem log_setup)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

FIND_PACKAGE(Threads)

if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
endif()

# Pull in the project includes
include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/builds/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/builds)
set(LIBS ${LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${RT_LIBRARY})

#création de groupes 
source_group(Server server/*.*)
source_group(Tools tools/*.*)

# Build the plugin experiment
add_executable(yadoms 
server/main.cpp
server/Supervisor.cpp
server/Supervisor.h
tools/ThreadBase.cpp
tools/ThreadBase.h
)

target_link_libraries(yadoms ${LIBS} ${CMAKE_DL_LIBS})
#add_library(awesomeplugin SHARED src/awesomeplugin.cpp)
#add_library(awesomeplugin2 SHARED src/awesomeplugin2.cpp)

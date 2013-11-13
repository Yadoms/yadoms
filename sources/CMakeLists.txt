# Project Stuff
cmake_minimum_required (VERSION 2.6)
project (PluginDemo)



# Default Options
if(UNIX AND NOT APPLE)
	add_definitions("-std=c++0x")
endif()

#suppress VS solution ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Find Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_LIBS thread)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

FIND_PACKAGE(Threads)

if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
endif()

# Pull in the project includes
include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBS ${LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${RT_LIBRARY})


# Build the plugin experiment
add_executable(pluginloader src/loader.cpp)
target_link_libraries(pluginloader ${LIBS} ${CMAKE_DL_LIBS})
add_library(awesomeplugin SHARED src/awesomeplugin.cpp)
add_library(awesomeplugin2 SHARED src/awesomeplugin2.cpp)
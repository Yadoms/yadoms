# Project Stuff
cmake_minimum_required (VERSION 2.6)
project (Yadoms)

#include user specific configuration
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

#RT Library
if(UNIX AND NOT APPLE)
    set(RT_LIBRARY rt)
endif()

# Pull in the project includes
include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/builds/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/builds)
set(LIBS ${LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${RT_LIBRARY})

# Create a shared library with common files shared by server and plugin
set(SHARED_LIB_SRC
tools/ThreadBase.cpp
tools/ThreadBase.h
)
add_library(sharedLib STATIC  ${SHARED_LIB_SRC})
target_link_libraries(sharedLib ${LIBS} ${CMAKE_DL_LIBS})

# Create a base shared library for Hardware plugins
set(HARDWARE_PLUGIN_BASE_LIB_SRC
plugins/hardware/HardwarePluginBase.cpp
plugins/hardware/HardwarePluginBase.h
)
add_library(hardwarePluginBaseLib STATIC  ${HARDWARE_PLUGIN_BASE_LIB_SRC})
target_link_libraries(hardwarePluginBaseLib ${LIBS} ${CMAKE_DL_LIBS})

#include Plugin CMake file
add_subdirectory(plugins)

#Create project directories
source_group(Server server/*.*)
source_group(Tools tools/*.*)

# Create main executable
set(YADOMS_SRC
server/main.cpp
server/Supervisor.cpp
server/Supervisor.h
)
add_executable(yadoms ${YADOMS_SRC})
target_link_libraries(yadoms sharedLib ${LIBS} ${CMAKE_DL_LIBS})

#configure solution directories (MSVC only)
if(MSVC)
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
	SET_PROPERTY(TARGET yadoms                	PROPERTY FOLDER "Server")
	SET_PROPERTY(TARGET sharedLib        		PROPERTY FOLDER "Shared")
	SET_PROPERTY(TARGET hardwarePluginBaseLib   PROPERTY FOLDER "Plugins")
endif()
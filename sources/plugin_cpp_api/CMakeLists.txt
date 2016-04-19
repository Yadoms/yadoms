# Make a static library to link with C++ plugins
#TODO plugin_cpp_api à garder ? ? ?


set(plugin_cpp_api_SOURCES
   plugin_cpp_api/stdafx.cpp
   plugin_cpp_api/stdafx.h
   plugin_cpp_api/PluginMain.h

   plugin_cpp_api/ICommandLine.h
   plugin_cpp_api/ImplementationHelper.h
   plugin_cpp_api/IPlugin.h
   plugin_cpp_api/IPluginContext.h

   plugin_cpp_api/ApiImplementation.h
   plugin_cpp_api/ApiImplementation.cpp
   plugin_cpp_api/CommandLine.h
   plugin_cpp_api/CommandLine.cpp

   plugin_cpp_api/PluginInformation.h
   plugin_cpp_api/PluginInformation.cpp

   plugin_cpp_api/PluginContext.h
   plugin_cpp_api/PluginContext.cpp
)

add_library(plugin_cpp_api STATIC ${plugin_cpp_api_SOURCES})

if(COTIRE_USE)
	#nothing to do
else()
	add_precompiled_header(plugin_cpp_api stdafx.h)
endif()


#define the list of all include dirs
set(plugin_cpp_api_ALL_INCLUDE_DIRS
   ${SHARED_INCL_DIR}
   ${BOOST_INCL_DIR}
   ${Poco_INCLUDE_DIRS}
   ${PROTOBUF_INCLUDE_DIRS}
   ${plugin_IPC_GENERATED_DIR}
   )

set_property(TARGET plugin_cpp_api PROPERTY INCLUDE_DIRECTORIES ${plugin_cpp_api_ALL_INCLUDE_DIRS})


target_link_libraries(plugin_cpp_api
   yadoms-shared
   ${LIBS}
   ${CMAKE_DL_LIBS}
   )


IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET plugin_cpp_api PROPERTY FOLDER "plugin_cpp_api")
ENDIF()

set(plugin_cpp_api_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

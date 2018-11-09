# Make a static library to link with C++ plugins

set(plugin_cpp_api_SOURCES
   stdafx.cpp
   stdafx.h

   ICommandLine.h
   IPluginContext.h

   ApiImplementation.h
   ApiImplementation.cpp
   CommandLine.h
   CommandLine.cpp

   BindingQuery.h
   BindingQuery.cpp
   BindingQueryData.h
   BindingQueryData.cpp
   DeviceCommand.h
   DeviceCommand.cpp
   DeviceConfigurationSchemaRequest.h
   DeviceConfigurationSchemaRequest.cpp
   DeviceRemoved.h
   DeviceRemoved.cpp
   ExtraQuery.h
   ExtraQuery.cpp
   ExtraQueryData.h
   ExtraQueryData.cpp
   Location.h
   Location.cpp
   ManuallyDeviceCreation.h
   ManuallyDeviceCreation.cpp
   ManuallyDeviceCreationData.h
   ManuallyDeviceCreationData.cpp
   PluginInformation.h
   PluginInformation.cpp
   SetDeviceConfiguration.h
   SetDeviceConfiguration.cpp
   YadomsInformation.h
   YadomsInformation.cpp

   PluginContext.h
   PluginContext.cpp

   PluginMain.cpp

   plugin_cpp_api/ImplementationHelper.h
   plugin_cpp_api/IPlugin.h
)

add_library(plugin_cpp_api STATIC ${plugin_cpp_api_SOURCES})


#define the list of all include dirs
set(plugin_cpp_api_ALL_INCLUDE_DIRS
   ${SHARED_INCL_DIR}
   ${BOOST_INCL_DIR}
   ${Poco_INCLUDE_DIRS}
   ${PROTOBUF_INCLUDE_DIRS}
   ${plugin_IPC_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}/plugin_cpp_api
   )

set_property(TARGET plugin_cpp_api PROPERTY INCLUDE_DIRECTORIES ${plugin_cpp_api_ALL_INCLUDE_DIRS})


target_link_libraries(plugin_cpp_api
   yadoms-shared
   ${LIBS}
   ${CMAKE_DL_LIBS}
   ${plugin_IPC_LIBRARY}
   )

##################################################################################################
## Add natvis (allow better debugging within VisualStudio)
##################################################################################################
ADD_VS_NATVIS(plugin_cpp_api)
   
if(COTIRE_USE)
   set_target_properties(plugin_cpp_api PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
   
   if(COTIRE_USE_UNITY)
      set_target_properties(plugin_cpp_api PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
   else()
      set_target_properties(plugin_cpp_api PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
   endif()
   
   cotire(plugin_cpp_api)
   
   if(COTIRE_USE_UNITY)
      target_link_libraries(plugin_cpp_api_unity yadoms-shared_unity ${LIBS} ${CMAKE_DL_LIBS} ${plugin_IPC_LIBRARY}_unity)
      ADD_VS_NATVIS(plugin_cpp_api_unity)
   endif()	
else()
	add_precompiled_header(plugin_cpp_api stdafx.h)
endif()

IF(MSVC OR XCODE)
   SET_PROPERTY(TARGET plugin_cpp_api PROPERTY FOLDER "plugin_cpp_api")
ENDIF()

set(plugin_cpp_api_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

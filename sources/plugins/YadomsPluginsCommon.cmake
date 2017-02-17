# Macros for setting up a plugin
#

MACRO(PLUGIN_SOURCES _targetName)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${YADOMS_OUTPUT_DIR}/plugins/${_targetName} )
   foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
       string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
       set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG}/plugins/${_targetName} )
   endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
   
   FILE(GLOB TRANSLATION_FILES locales/*)
   source_group(locales locales/*)
   
   set(PLUGIN_SOURCE_FILES
      ${ARGN}
      package.json
      ${TRANSLATION_FILES}
      )
      
   add_executable(${_targetName} ${PLUGIN_SOURCE_FILES})
   project(${_targetName})
	
	IF(MSVC OR XCODE)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "plugins")
	ENDIF()
ENDMACRO()

MACRO(PLUGIN_INCLDIR _targetName)

   #define the list of all include dirs
   set(PLUGINS_ALL_INCLUDE_DIRS
      ${SHARED_INCL_DIR}
      ${BOOST_INCL_DIR}
      ${Poco_INCLUDE_DIRS}
      ${plugin_cpp_api_INCLUDE_DIR}
      ${ARGN}
      )
         
   #in case of OpenSSL found, just add openssl include dir
   if(${OPENSSL_FOUND})
      set(PLUGINS_ALL_INCLUDE_DIRS  ${PLUGINS_ALL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
   endif()

	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${PLUGINS_ALL_INCLUDE_DIRS})
ENDMACRO()

MACRO(PLUGIN_LINK _targetName)
	target_link_libraries(${_targetName}
      yadoms-shared
      plugin_cpp_api
      ${LIBS}
      ${CMAKE_DL_LIBS}
      ${PROTOBUF_LIBRARIES}
      ${ARGN}
      )
	
   
   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   
   #configure plugin as installable component
	install(TARGETS ${_targetName} 
		RUNTIME DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}
		COMPONENT  ${ComponentCompatibleName})
      
   set(PLUGINLIST
      ${PLUGINLIST}
      ${_targetName}
      PARENT_SCOPE)
     

	if(COTIRE_USE)
		set_target_properties(${_targetName} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
		
		if(COTIRE_USE_UNITY)
			set_target_properties(${_targetName} PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
		else()
			set_target_properties(${_targetName} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
		endif()
		
		cotire(${_targetName})
		
		if(COTIRE_USE_UNITY)
			target_link_libraries(${_targetName}_unity yadoms-shared_unity ${LIBS} ${CMAKE_DL_LIBS} ${ARGN})
		endif()	
	endif()	
	
ENDMACRO()

# brief Copy a file to the target output dir
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource (absolute path) to copy to the target output dir
MACRO(PLUGIN_POST_BUILD_COPY_FILE _targetName _resource)

   get_filename_component(_resourcePath ${_resource}  DIRECTORY)
   get_filename_component(_resourceFile ${_resource}  NAME)
   
   if (_resourcePath STREQUAL "")
      set(_resourcePath ${CMAKE_CURRENT_SOURCE_DIR})
   endif()

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   
   install(FILES ${_resourcePath}/${_resourceFile} 
			DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}/${_resourcePath}
			COMPONENT  ${ComponentCompatibleName})
			
   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_resourcePath}/${_resourceFile} $<TARGET_FILE_DIR:${_targetName}>/${_resourceFile})
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_resourcePath}/${_resourceFile} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resourceFile})
	  endif()	
   endif()	
ENDMACRO()



# brief Copy a dependency (dll or so) to yadoms output dir (not next to _targetName)
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource (absolute path) to copy to Yadoms dir
MACRO(PLUGIN_POST_BUILD_COPY_FILE_DEPENDENCY _targetName _resource)

   get_filename_component(_resourcePath ${_resource}  DIRECTORY)
   get_filename_component(_resourceFile ${_resource}  NAME)
   
   if (_resourcePath STREQUAL "")
      set(_resourcePath ${CMAKE_CURRENT_SOURCE_DIR})
   endif()
   
   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   
   install(FILES ${_resourcePath}/${_resourceFile} 
			DESTINATION ${_resourcePath}
			COMPONENT  ${ComponentCompatibleName})
			
   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_resourcePath}/${_resourceFile} $<TARGET_FILE_DIR:yadoms>/)
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_resourcePath}/${_resourceFile} $<TARGET_FILE_DIR:yadoms_unity>/)
	  endif()	
   endif()	
ENDMACRO()

# brief Copy list of dependencies (dll or so) to yadoms output dir (not next to _targetName)
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resources The resources (absolute path) to copy to Yadoms dir
MACRO(PLUGIN_POST_BUILD_COPY_FILE_DEPENDENCIES _targetName _resources)
   foreach(resource ${_resources})
      PLUGIN_POST_BUILD_COPY_FILE_DEPENDENCY(${_targetName} ${resource})
   endforeach(resource)
ENDMACRO()

# brief Copy a directory (and its content) to the target output dir
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource folder (absolute path) to copy to the target output dir
MACRO(PLUGIN_POST_BUILD_COPY_DIRECTORY _targetName _resource)
   
   if (_resource STREQUAL "")
      set(_resource ${CMAKE_CURRENT_SOURCE_DIR})
   endif()

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})

   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} 
			DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}
			COMPONENT  ${ComponentCompatibleName})

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${_resource} $<TARGET_FILE_DIR:${_targetName}>/${_resource})
	  
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resource})
	  endif()	
   endif()		  
ENDMACRO()

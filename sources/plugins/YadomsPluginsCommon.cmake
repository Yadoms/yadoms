# Macros for setting up a plugin
#

MACRO(PLUGIN_SOURCES _targetName)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${YADOMS_OUTPUT_DIR}/plugins/${_targetName} )
   foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
       string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
       set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG}/plugins/${_targetName})
   endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
   
   FILE(GLOB TRANSLATION_FILES locales/*)
   source_group(locales locales/*)

   # Add package.json if exists (can be absent if generated)
   FILE(GLOB PACKAGE_JSON_FILE package.json)
   
   set(PLUGIN_SOURCE_FILES
      ${ARGN}
      ${PACKAGE_JSON_FILE}
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
		
	endif()	
	
ENDMACRO()

# brief Copy a file to the target output dir
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource (absolute path) to copy to the target output dir
MACRO(PLUGIN_POST_BUILD_COPY_FILE _targetName _resource)

   if (IS_ABSOLUTE ${_resource})
      set(resource ${_resource})
   else()
      set(resource "${CMAKE_CURRENT_SOURCE_DIR}/${_resource}")
   endif()

   get_filename_component(_resourcePath ${resource}  DIRECTORY)
   get_filename_component(_resourceFile ${resource}  NAME)

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})

   install(FILES ${resource} 
			DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}
			COMPONENT  ${ComponentCompatibleName})
			
   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${resource} $<TARGET_FILE_DIR:${_targetName}>/${_resourceFile})
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resourceFile})
	  endif()	
   endif()	
ENDMACRO()

# brief Copy a directory (and its content) to the target output dir
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource folder (absolute path) to copy to the target output dir
# param [in/optional] To specify the target destination (if omit, destination will be the last part of _resource tree)
MACRO(PLUGIN_POST_BUILD_COPY_DIRECTORY _targetName _resource)

   set (extra_args ${ARGN})
   list(LENGTH extra_args num_extra_args)
   if (${num_extra_args} GREATER 0)
      # A target folder is given
      list(GET extra_args 0 where)
   else()
      # No target folder
      if (IS_ABSOLUTE ${_resource})
         get_filename_component(where "${_resource}" NAME)
      else()
         set(where ${_resource})
      endif()
   endif()
   
   if (IS_ABSOLUTE ${_resource})
      set(resource ${_resource})
   else()
      set(resource "${CMAKE_CURRENT_SOURCE_DIR}/${_resource}")
   endif()

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})

   install(DIRECTORY ${resource}
			DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}/
			COMPONENT  ${ComponentCompatibleName})

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${resource} $<TARGET_FILE_DIR:${_targetName}>/${where})
	  
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy_directory ${resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${where})
	  endif()	
   endif()		  
ENDMACRO()

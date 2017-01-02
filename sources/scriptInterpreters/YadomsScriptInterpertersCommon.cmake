# Macros for setting up a script interpreter
#

MACRO(SCRIPT_INTERPRETER_SOURCES _targetName)
   set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${YADOMS_OUTPUT_DIR}/scriptInterpreters/${_targetName} )
   foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
       string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
       set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG}/scriptInterpreters/${_targetName} )
   endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

   set(SCRIPT_INTERPRETER_SOURCE_FILES
      ${ARGN}
      package.json
      )
      
   add_executable(${_targetName} ${SCRIPT_INTERPRETER_SOURCE_FILES})
   project(${_targetName})
	
	IF(MSVC OR XCODE)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "scriptInterpreters/${_targetName}")
	ENDIF()
ENDMACRO()

MACRO(SCRIPT_INTERPRETER_INCLDIR _targetName)

   #define the list of all include dirs
   set(SCRIPT_INTERPRETER_ALL_INCLUDE_DIRS
      ${SHARED_INCL_DIR}
      ${BOOST_INCL_DIR}
      ${Poco_INCLUDE_DIRS}
      ${interpreter_cpp_api_INCLUDE_DIR}
      ${ARGN}
      )


	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${SCRIPT_INTERPRETER_ALL_INCLUDE_DIRS})
ENDMACRO()

MACRO(SCRIPT_INTERPRETER_LINK _targetName)
	target_link_libraries(${_targetName}
      yadoms-shared
      interpreter_cpp_api
      ${LIBS}
      ${CMAKE_DL_LIBS}
      ${PROTOBUF_LIBRARIES}
      ${interpreter_IPC_LIBRARY}
      ${ARGN}
      )
	

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   
   #configure interpreter as installable component
	install(TARGETS ${_targetName} 
		RUNTIME DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}
		COMPONENT  ${ComponentCompatibleName})
		
   set(SCRIPTINTERPRETERSLIST
      ${SCRIPTINTERPRETERSLIST}
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
# param [in] _targetName The current target (ie: interpreterName)
# param [in] _resource The resource (absolute path) to copy to the target output dir
MACRO(SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE _targetName _resource)

   get_filename_component(_resourcePath ${_resource}  DIRECTORY)

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})

   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} 
			DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}/${_resourcePath}
			COMPONENT   ${ComponentCompatibleName})

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}>/${_resource})
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resource})
	  endif()	
   endif()	
ENDMACRO()


# brief Copy a directory (and its content) to the target output dir
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource folder (absolute path) to copy to the target output dir
MACRO(SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY _targetName _resource)
   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} 
			DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}
			COMPONENT   ${ComponentCompatibleName})

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}>/${_resource})
	  
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resource})
	  endif()	
   endif()		  
ENDMACRO()

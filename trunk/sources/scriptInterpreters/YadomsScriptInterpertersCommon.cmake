# Macros for setting up a script interpreter
#

MACRO(SCRIPT_INTERPRETER_SOURCES _targetName)
   set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${youroutputdirectory}/scriptInterpreters/${_targetName} )
   foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
       string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
       set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${youroutputdirectory}/${OUTPUTCONFIG}/scriptInterpreters/${_targetName} )
   endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
	add_library(${_targetName} MODULE ${ARGN})
	
	IF(MSVC OR XCODE)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "scriptInterpreters/${_targetName}")
	ENDIF()
ENDMACRO()

MACRO(SCRIPT_INTERPRETER_INCLDIR _targetName)
	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${Poco_INCLUDE_DIRS} ${ARGN})
ENDMACRO()

MACRO(SCRIPT_INTERPRETER_LINK _targetName)
	target_link_libraries(${_targetName} yadoms-shared ${LIBS} ${CMAKE_DL_LIBS} ${ARGN})
	
	install(TARGETS ${_targetName} 
		LIBRARY DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}
		COMPONENT  scriptInterpreters)
		
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

MACRO(SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE _targetName _resource)

   get_filename_component(_resourcePath ${_resource}  DIRECTORY)

   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} 
			DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}/${_resourcePath}
			COMPONENT  scriptInterpreters)

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}>/${_resource})
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resource})
	  endif()	
   endif()	
ENDMACRO()

MACRO(SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY _targetName _resource)
   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} 
			DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}
			COMPONENT  scriptInterpreters)

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}>/${_resource})
	  
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${_resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resource})
	  endif()	
   endif()		  
ENDMACRO()

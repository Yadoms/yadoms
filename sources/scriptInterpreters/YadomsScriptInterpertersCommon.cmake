# Macros for setting up a script interpreter
#

function(SCRIPT_INTERPRETER_IS_IN_DEV_STATE _targetName)
   string(FIND ${_targetName} "dev-" DEV_SUBSTRING_POSITION)
   if (${DEV_SUBSTRING_POSITION} EQUAL 0)
      set(DEV_SCRIPT_INTERPRETER TRUE PARENT_SCOPE)
   else()
      set(DEV_SCRIPT_INTERPRETER FALSE PARENT_SCOPE)
   endif()
endfunction()

MACRO(SCRIPT_INTERPRETER_SOURCES _targetName)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${YADOMS_OUTPUT_DIR}/scriptInterpreters/${_targetName} )
   foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
       string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
       set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG}/scriptInterpreters/${_targetName})
   endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
   
   FILE(GLOB TRANSLATION_FILES locales/*)
   source_group(locales locales/*)

   set(SCRIPT_INTERPRETER_SOURCE_FILES
      ${ARGN}
      package.json
      changelog.md
      ${TRANSLATION_FILES}
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
      ${ARGN}
      )
	
   ADD_VS_NATVIS(${_targetName})

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   
   #configure interpreter as installable component
   SCRIPT_INTERPRETER_IS_IN_DEV_STATE(${_targetName})
   if (NOT ${DEV_SCRIPT_INTERPRETER})
      install(TARGETS ${_targetName} 
         RUNTIME DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}
         COMPONENT  ${ComponentCompatibleName})
   endif()
   ##################################################################################################
   ## RPATH
   ##################################################################################################
   if(CMAKE_CROSSCOMPILING)
      #Fix RPATH for cross compilation
      set_target_properties(${_targetName} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
   endif(CMAKE_CROSSCOMPILING)
		
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
			target_link_libraries(${_targetName}_unity yadoms-shared_unity interpreter_cpp_api_unity ${LIBS} ${CMAKE_DL_LIBS} ${PROTOBUF_LIBRARIES} ${ARGN})
         ADD_VS_NATVIS(${_targetName}_unity)
		   if(CMAKE_CROSSCOMPILING)
		      #Fix RPATH for cross compilation
		      set_target_properties(${_targetName}_unity PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
		   endif(CMAKE_CROSSCOMPILING)
		endif()	
	endif()	
	
ENDMACRO()

# brief Copy a file to the target output dir
# param [in] _targetName The current target (ie: interpreterName)
# param [in] _resource The resource (absolute path) to copy to the target output dir
MACRO(SCRIPT_INTERPRETER_POST_BUILD_COPY_FILE _targetName _resource)

   if (IS_ABSOLUTE ${_resource})
      set(resource ${_resource})
   else()
      set(resource "${CMAKE_CURRENT_SOURCE_DIR}/${_resource}")
   endif()

   get_filename_component(_resourcePath ${resource}  DIRECTORY)
   get_filename_component(_resourceFile ${resource}  NAME)

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   
   SCRIPT_INTERPRETER_IS_IN_DEV_STATE(${_targetName})
   if (NOT ${DEV_SCRIPT_INTERPRETER})
      install(FILES ${resource} 
         DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}
         COMPONENT  ${ComponentCompatibleName})
   endif()
			
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
MACRO(SCRIPT_INTERPRETER_POST_BUILD_COPY_DIRECTORY _targetName _resource)

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

   SCRIPT_INTERPRETER_IS_IN_DEV_STATE(${_targetName})
   if (NOT ${DEV_SCRIPT_INTERPRETER})
      install(DIRECTORY ${resource} 
			DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/${_targetName}/
			COMPONENT  ${ComponentCompatibleName})
   endif()

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${resource} $<TARGET_FILE_DIR:${_targetName}>/${where})
	  
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${where})
	  endif()	
   endif()		  
ENDMACRO()
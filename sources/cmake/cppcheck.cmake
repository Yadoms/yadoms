

macro(create_cppcheck_command projectName cppcheck_executable cppcheck_report_file)

message(STATUS "  Create CppCheck command line")
		
		get_cppcheck_defines(CPPCHECK_DEFINES ${projectName})
		get_cppcheck_sources(CPPCHECK_SOURCES ${projectName})
		get_cppcheck_includes(CPPCHECK_INCLUDE_DIRECTORIES "${YADOMS_INCL_DIR}")
		
      add_custom_command(
         OUTPUT ${cppcheck_report_file}
         BYPRODUCTS ${cppcheck_report_file}
         COMMAND ${cppcheck_executable}
            --enable=all
            --std=c++11
            --xml
            --xml-version=2
            --verbose
            --quiet
            --suppress=missingIncludeSystem 
            --suppress=*:*.pb.h
            #--check-config       ## Enable this line only to find missing include (don't do analysis otherwise)
            ${CPPCHECK_DEFINES}
            ${CPPCHECK_INCLUDE_DIRECTORIES}
            ${CPPCHECK_EXCLUDE_DIRECTORIES}
            ${CPPCHECK_SOURCES}
            2> ${cppcheck_report_file}
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      )
	
endmacro()

MACRO(get_cppcheck_sources output projectName)

	get_target_property(sources ${projectName} SOURCES)
   
   # Remove .h files
   LIST(FILTER sources EXCLUDE REGEX "^.*\.h$")
      
   if(MSVC)
      # Remove .rc files
      LIST(FILTER sources EXCLUDE REGEX "^.*\.rc$")
   endif()
      
	SET(${output} ${sources})
	
ENDMACRO()

MACRO(get_cppcheck_includes output projectIncludeDirectories)
   
   set(INCS)
   foreach (includeDirectory ${projectIncludeDirectories})
      STRING(CONCAT includeDirectory "-I\"" "${includeDirectory}" "\"")
      set(INCS ${INCS} ${includeDirectory})
	endforeach()
   set(includeDirectories ${INCS})
   
	SET(${output} ${includeDirectories})

ENDMACRO()

MACRO(get_cppcheck_defines output projectName)

	get_directory_property(COMPILE_DEFS DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
	get_target_property(targetDefines ${projectName} COMPILE_DEFINITIONS)
	IF (targetDefines)
		set(COMPILE_DEFS ${COMPILE_DEFS} ${targetDefines})
	ENDIF()
   
   set(DEFS)
   foreach (compileDef ${COMPILE_DEFS})
      STRING(CONCAT compileDef "-D\"" "${compileDef}" "\"")
      set(DEFS ${DEFS} ${compileDef})
	endforeach()
   set(COMPILE_DEFS ${DEFS})
	
	SET(${output} ${COMPILE_DEFS})
	
ENDMACRO()

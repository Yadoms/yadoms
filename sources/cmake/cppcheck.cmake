

macro(create_cppcheck_command projectName cppcheck_executable cppcheck_report_file)

message(STATUS "  Create CppCheck command line")
		
		# TODO remettre get_sonarqube_defines(SONARQUBE_DEFINES ${projectName})
		get_cppcheck_sources(CPPCHECK_SOURCES ${projectName})
		get_cppcheck_includes(CPPCHECK_INCLUDE_DIRECTORIES ${projectName})
		# get_sonarqube_tests(SONARQUBE_TESTS ${projectName})
		
      add_custom_command(
         OUTPUT ${cppcheck_report_file}
         BYPRODUCTS ${cppcheck_report_file}
         COMMAND ${cppcheck_executable}
            --enable=all
            --std=c++11
            --xml
            --verbose
            --quiet
            ${CPPCHECK_INCLUDE_DIRECTORIES}
            ${CPPCHECK_SOURCES}
            2> ${cppcheck_report_file}
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      )
	
endmacro()

#TODO faire ménage dans tout ce qui suit

macro(get_msbuild_entry output key value)

	set(${output} "<Property Name=\"${key}\">${value}</Property>")

endmacro()

MACRO(get_sonarqube_tests output projectName)

	get_target_property(testTagets ${projectName} TESTED_BY)
	set(SONARQUBE_JUNIT_RESULTS)
	foreach (testTarget ${testTargets})
		get_target_property(kind ${testTarget} TEST_FRAMEWORK)
		get_target_property(reportPath ${testTarget} TEST_REPORT)
		if ("${kind}" STREQUAL "GTest")
			set(SONARQUBE_JUNIT_RESULTS ${SONARQUBE_JUNIT_RESULTS} ${reportPath})
		elseif ("${kind}" STREQUAL "NUnit")
			get_target_property(reportPath ${testTarget} TEST_REPORT_JUNIT)
			set(SONARQUBE_JUNIT_RESULTS ${SONARQUBE_JUNIT_RESULTS} ${reportPath})
		else()
			message("Impossible de déterminer le type de test pour ${testProject}")
		endif()
	endforeach()
	
	if (SONARQUBE_JUNIT_RESULTS)
		JOIN("${SONARQUBE_JUNIT_RESULTS}" "," SONARQUBE_JUNIT_RESULTS)
		set(${output} sonar.cxx.xunit.reportPath=${SONARQUBE_JUNIT_RESULTS})
	endif()
	

ENDMACRO()

MACRO(get_cppcheck_sources output projectName)

	get_target_property(sources ${projectName} SOURCES)
   if(MSVC)
      # Remove .rc files
      LIST(FILTER sources EXCLUDE REGEX "^.*\.rc$")
   endif()
      
	SET(${output} ${sources})
	
ENDMACRO()

MACRO(get_cppcheck_includes output projectName)

	# TODO voir si utile get_std_headers_path(stdHeaders)
	get_target_property(includeDirectories ${projectName} INCLUDE_DIRECTORIES)
	set(includeDirectories ${includeDirectories}
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"#TODO ne pas laisser en dur
		"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"#TODO ne pas laisser en dur
		${stdHeaders}
	)
   
   set(INCS)
   foreach (includeDirectory ${includeDirectories})
      STRING(CONCAT includeDirectory "-I\"" "${includeDirectory}" "\"")
      set(INCS ${INCS} ${includeDirectory})
	endforeach()
   set(includeDirectories ${INCS})
   
	SET(${output} ${includeDirectories})

ENDMACRO()

MACRO(get_sonarqube_defines output projectName)

	get_directory_property(COMPILE_DEFS DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
	get_target_property(targetDefines ${projectName} COMPILE_DEFINITIONS)
	IF (targetDefines)
		set(COMPILE_DEFS ${COMPILE_DEFS} ${targetDefines})
	ENDIF()
	
	
	STRING(REGEX REPLACE "=" " " COMPILE_DEFS "${COMPILE_DEFS}" )
	STRING(REPLACE ";" "\\\\n\\\\\\\n" COMPILE_DEFS "${COMPILE_DEFS}" )
	
	SET(${output} ${COMPILE_DEFS})
	
ENDMACRO()

MACRO(get_sonarqube_coverage output projectName)

	get_target_property(testTagets ${projectName} TESTED_BY)
	set(SONARQUBE_UNIT)
	set(SONARQUBE_ACCEPTANCE)
	foreach (testTarget ${testTargets})
		get_target_property(kind ${testTarget} TEST_KIND)
		get_target_property(reportPath ${testTarget} COVERAGE_REPORT_XML)
		if ("${kind}" STREQUAL "UNIT")
			set(SONARQUBE_UNIT ${SONARQUBE_UNIT} ${reportPath})
		elseif ("${kind}" STREQUAL "ACCEPTANCE")
			set(SONARQUBE_ACCEPTANCE ${SONARQUBE_ACCEPTANCE} ${reportPath})
		else()
			message("Impossible de déterminer le type de test pour ${testProject} (ACCEPTANCE ou UNIT)")
		endif()
	endforeach()
	
	SET(COVERAGE_UNIT_STEPS)
	SET(COVERAGE_ACCEPTANCE_STEPS)
	SET(COVERAGE_ALL_STEPS)
	if (SONARQUBE_UNIT)
		JOIN("${SONARQUBE_UNIT}" "," SONARQUBE_UNIT)
		set(COVERAGE_UNIT_STEPS sonar.cxx.coverage.reportPath=${SONARQUBE_UNIT})
	endif()
	if (SONARQUBE_ACCEPTANCE)
		JOIN("${SONARQUBE_ACCEPTANCE}" "," SONARQUBE_ACCEPTANCE)
		set(COVERAGE_UNIT_STEPS sonar.cxx.coverage.itReportPath=${SONARQUBE_ACCEPTANCE})
	endif()
	IF (SONARQUBE_UNIT OR SONARQUBE_ACCEPTANCE)
		get_target_property(report ${projectName} COVERAGE_REPORT_XML_ALL)
		SET(COVERAGE_ALL_STEPS sonar.cxx.coverage.overallReportPath=${report})
	ENDIF()
	
	SET(${output} ${COVERAGE_UNIT_STEPS} ${COVERAGE_ACCEPTANCE_STEPS} ${COVERAGE_ALL_STEPS})
	
ENDMACRO()


MACRO(get_root output)
   set(${output} "D:/Projets/Domotique/yadoms") #TODO en dur pour test
ENDMACRO()


# Converts a CMake list to a string containing elements separated by a separator
MACRO(list_to_string output)
   set(${output})
   set(separator)
   FOREACH(line ${ARGN})
      set(${output} "${${output}}${separator}${line}")
      set(separator "\n")
   ENDFOREACH(line)
ENDMACRO()


function(JOIN VALUES GLUE OUTPUT)
  string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

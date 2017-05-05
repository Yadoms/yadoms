

macro(create_sonar_project_properties_file sonarQubeKey target projectVersion sources outputFile)

message("Create SonarQube configuration file ${outputFile} for target ${target}(v${projectVersion})")

		set(SONARQUBE_KEY ${sonarQubeKey})
		set(SONARQUBE_PROJECT_NAME ${target})
		
      set(SONARQUBE_VERSION ${projectVersion})
		get_sonarqube_defines(SONARQUBE_DEFINES ${target})
		get_sonarqube_sources(SONARQUBE_SOURCES ${target} "${sources}")
		get_sonarqube_includes(SONARQUBE_INCLUDES ${target})
		get_sonarqube_tests(SONARQUBE_TESTS ${target})
		get_sonarqube_dr_memory(SONARQUBE_DR_MEMORY ${target})
		get_sonarqube_coverage(SONARQUBE_COVERAGE ${target})
		
		list_to_string(CONTENT
			sonar.projectKey=${SONARQUBE_KEY}
			sonar.projectVersion=${SONARQUBE_VERSION}
			sonar.projectName=${SONARQUBE_PROJECT_NAME}
			sonar.language=c++
			sonar.sources=${SONARQUBE_SOURCES}
			sonar.cxx.coverage.forceZeroCoverage=False
			sonar.cxx.includeDirectories=${SONARQUBE_INCLUDES}
			${SONARQUBE_TESTS}
			${SONARQUBE_DR_MEMORY}
			${SONARQUBE_COVERAGE}
		)
		set(CONTENT "${CONTENT}\nsonar.cxx.defines=${SONARQUBE_DEFINES}")
	
		FILE(WRITE ${outputFile} ${CONTENT})
		
		get_msbuild_entry(SONAR_LANGUAGE sonar.language c++)
		get_msbuild_entry(SONAR_SOURCES sonar.inclusions  ${SONARQUBE_SOURCES})
		get_msbuild_entry(SONAR_FORCE_ZERO_COVERAGE sonar.cxx.coverage.forceZeroCoverage False)
		get_msbuild_entry(SONAR_INCLUDE_DIRECTORIES sonar.cxx.includeDirectories ${SONARQUBE_INCLUDES})
		get_msbuild_entry(SONAR_HOST sonar.host.url http://localhost:9000)
		get_root(LOCAL_REPO_PATH)
		
		
		get_target_property(testTagets ${target} TESTED_BY)
		set(unitReportPaths)
		SET(i 1)
		foreach (testTarget ${testTargets})
			set(unitReportPaths ${unitReportPaths} ${LOCAL_REPO_PATH}/TestResultsSQ/File${i}.trx)
			MATH(EXPR i "${i}+1")
		ENDFOREACH()
		JOIN("${unitReportPaths}" "," unitReportPaths)
		
		if (unitReportPaths)
		get_msbuild_entry(SONAR_TESTS sonar.cxx.vstest.reportsPaths ${unitReportPaths})
		endif()
		# get_target_property(externalSources ${target} EXTERNAL_SOURCES)
		# get_root(LOCAL_REPO_PATH)
		# STRING(REGEX REPLACE "${LOCAL_REPO_PATH}/" "" externalSources "${externalSources}" )
		# STRING(REGEX REPLACE ";" "," externalSources "${externalSources}" )
		# get_msbuild_entry(SONAR_HOST sonar.host.url http://localhost:9000)
		
		#get_msbuild_entry(COVERAGE_UNIT_TESTS sonar.cxx.coverage.reportPath UnitTestCoverageResults.xml)
		#get_msbuild_entry(COVERAGE_ACCEPTANCE_TESTS sonar.cxx.coverage.itReportPath AcceptanceTestCoverageResults.xml)
		get_msbuild_entry(COVERAGE_ALL_TESTS sonar.cxx.coverage.overallReportPath ${LOCAL_REPO_PATH}/AllTestCoverageResults.xml)
		get_msbuild_entry(COVERAGE_ACCEPTANCE_TESTS sonar.cxx.coverage.itReportPath ${LOCAL_REPO_PATH}/AllTestCoverageResults.xml)
		get_msbuild_entry(CPPCHECK_REPORTS sonar.cxx.cppcheck.reportPath *_cppcheck.xml)
	
endmacro()



macro(get_msbuild_entry output key value)

	set(${output} "<Property Name=\"${key}\">${value}</Property>")

endmacro()

MACRO(get_sonarqube_tests output target)

	get_target_property(testTagets ${target} TESTED_BY)
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

MACRO(get_sonarqube_includes output target)

	# TODO voir si utile get_std_headers_path(stdHeaders)
	get_target_property(targetIncludes ${target} INCLUDE_DIRECTORIES)
	set(targetIncludes ${targetIncludes}
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"#TODO ne pas laisser en dur
		"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"#TODO ne pas laisser en dur
		${stdHeaders}
	)
	STRING(REGEX REPLACE "${LOCAL_REPO_PATH}/" "" INCS "${targetIncludes}" )
	STRING(REGEX REPLACE ";" "," INCS "${INCS}" )
	SET(${output} ${INCS})

ENDMACRO()

MACRO(get_sonarqube_defines output target)

	get_directory_property( COMPILE_DEFS DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
	get_target_property(targetDefines ${target} COMPILE_DEFINITIONS)
	IF (targetDefines)
		set(COMPILE_DEFS ${COMPILE_DEFS} ${targetDefines})
	ENDIF()
	
	
	STRING(REGEX REPLACE "=" " " COMPILE_DEFS "${COMPILE_DEFS}" )
	STRING(REPLACE ";" "\\\\n\\\\\\\n" COMPILE_DEFS "${COMPILE_DEFS}" )
	
	SET(${output} ${COMPILE_DEFS})
	
ENDMACRO()

MACRO(get_sonarqube_sources output target projectSources)

	SET(SRCS)
	foreach (sourceFile ${projectSources})
		if ("${sourceFile}" MATCHES "^.*\\.(c|cpp)$")
			get_filename_component(sourceFile ${sourceFile} REALPATH)
			GET_SOURCE_FILE_PROPERTY(notASonarSource ${sourceFile} NOT_A_SONARQUBE_SOURCE_FOR_${target})
			if (NOT notASonarSource)
				set(SRCS ${SRCS} ${sourceFile})
			endif()
		endif()
	endforeach()
	IF (SRCS)
		list(REMOVE_DUPLICATES SRCS)
	ENDIF()

	get_root(LOCAL_REPO_PATH)
	STRING(REGEX REPLACE "${LOCAL_REPO_PATH}/" "" SRCS "${SRCS}" )
	STRING(REGEX REPLACE ";" "," SRCS "${SRCS}" )
	SET(${output} ${SRCS})
	
ENDMACRO()

MACRO(get_sonarqube_coverage output target)

	get_target_property(testTagets ${target} TESTED_BY)
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
		get_target_property(report ${target} COVERAGE_REPORT_XML_ALL)
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



MACRO(analyse target projectVersion sources outputFile outputFileMSBuild)

   # Find analysis dependancies
   # - CppCheck
   set(CPPCHECK_VERSION_MINIMAL 1.78)
   include(cmake/FindCppCheck.cmake)
   # - SonarQube scanner
   set(SONARSCANNER_VERSION_MINIMAL 2.9.0)
   include(cmake/FindSonarScanner.cmake)
   
   if(NOT CPPCHECK_FOUND)
      message(WARNING "Suitable CppCheck not found, skip analysis solution generation")
   elseif(NOT SONARSCANNER_FOUND)
      message(WARNING "Suitable SonarQube scanner not found, skip analysis solution generation")
   else()   
      message(STATUS "All analysis tools found, generate Analysis target...")
            
      # Add CppCheck analysis
      include(cmake/cppcheck.cmake)
      create_cppcheck_command(
            ${target}
            ${CPPCHECK_EXECUTABLE}
            ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
         )
      
      # Build SonarQube properties file
      include(cmake/sonarqube.cmake)
      create_sonar_project_properties_file(
            ${target}
            ${YADOMS_VERSION}
            ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
            ${CMAKE_BINARY_DIR}/analysis/sonar-project.properties
         )
      
      # Create the analysis target
      add_custom_target(Analysis
         DEPENDS ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
                 ${CMAKE_BINARY_DIR}/analysis/sonar-project.properties
         COMMAND ${SONARSCANNER_EXECUTABLE}
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/analysis/
      )

   endif()
   

	
ENDMACRO()

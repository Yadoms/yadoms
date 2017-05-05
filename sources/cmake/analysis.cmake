

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
      add_custom_command(
         OUTPUT ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
         BYPRODUCTS ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
         COMMAND ${CPPCHECK_EXECUTABLE}
            --enable=all
            --std=c++11
            --xml
            --verbose
            --quiet ${sources}
            2> ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      )
      
      # Build SonarQube properties file
      include(cmake/sonarqube.cmake)
      create_sonar_project_properties_file(
            ${target}
            ${YADOMS_VERSION}
            "${sources}"
            ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
            ${CMAKE_BINARY_DIR}/analysis/sonar-project.properties
         )
      
      # Create the analysis target
      add_custom_target(Analysis
         DEPENDS ${CMAKE_BINARY_DIR}/analysis/cppcheck-report.xml
                 ${CMAKE_BINARY_DIR}/analysis/sonar-project.properties
         COMMAND ${SONARSCANNER_EXECUTABLE}
      )

   endif()
   

	
ENDMACRO()

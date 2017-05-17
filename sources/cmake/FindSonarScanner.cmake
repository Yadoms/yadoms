# FindSonarScanner
# --------
#
# Find SonarQube scanner executable
#
# This module finds an installed SonarQube scanner.
#
# It takes as inputs :
#
#   SONARSCANNER_VERSION_MINIMAL - Minimal SonarQube scanner version to use
#
#
# It sets the following variables:
#
#   SONARSCANNER_FOUND - set to true if SonarQube scanner is found
#   SONARSCANNER_DIR - the directory where SonarQube scanner is installed
#   SONARSCANNER_EXECUTABLE - the path to the SonarQube scanner executable
#   SONARSCANNER_VERSION   - the version number of the SonarQube scanner executable
#
#



find_program(SONARSCANNER_EXECUTABLE
   NAMES sonar-scanner.bat sonar-scanner
   PATHS ${SONARQUBE_SCANNER_ROOT}/bin
)
 
if(SONARSCANNER_EXECUTABLE)
   execute_process(COMMAND ${SONARSCANNER_EXECUTABLE} --version
      OUTPUT_VARIABLE SONARSCANNER_VERSION_STRING
      ERROR_VARIABLE SONARSCANNER_VERSION_ERROR
      RESULT_VARIABLE SONARSCANNER_VERSION_RESULT)
      
   get_filename_component(SONARSCANNER_DIR ${SONARSCANNER_EXECUTABLE} DIRECTORY)

   if(SONARSCANNER_VERSION_RESULT)
      message(WARNING "Error getting SonarQube scanner version : ${SONARSCANNER_VERSION_ERROR}")
   else()
      string(REGEX REPLACE ".*SonarQube Scanner ([0-9.]*).*" "\\1" CPPCHECK_VERSION "${SONARSCANNER_VERSION_STRING}") 
      message(STATUS "Found SonarQube scanner version : ${CPPCHECK_VERSION}")
      if(${CPPCHECK_VERSION} VERSION_LESS ${CPPCHECK_VERSION_MINIMAL})
         message(WARNING "Wrong SonarQube scanner version : ${CPPCHECK_VERSION} found, ${CPPCHECK_VERSION_MINIMAL} minimal required")
      endif()
     endif()

   include(FindPackageHandleStandardArgs)
   FIND_PACKAGE_HANDLE_STANDARD_ARGS(SONARSCANNER REQUIRED_VARS SONARSCANNER_EXECUTABLE SONARSCANNER_DIR
                                     VERSION_VAR SONARSCANNER_VERSION)
                                     
endif()

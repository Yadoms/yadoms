# FindCppCheck
# --------
#
# Find CppCheck executable
#
# This module finds an installed CppCheck.
#
# It takes as inputs :
#
#   CPPCHECK_VERSION_MINIMAL - Minimal CppCheck version to use
#
#
# It sets the following variables:
#
#   CPPCHECK_FOUND - set to true if CppCheck is found
#   CPPCHECK_DIR - the directory where CppCheck is installed
#   CPPCHECK_EXECUTABLE - the path to the CppCheck executable
#   CPPCHECK_VERSION   - the version number of the CppCheck executable
#
#



find_program(CPPCHECK_EXECUTABLE cppcheck
   PATHS ${CPPCHECK_ROOT}
)
 
if(CPPCHECK_EXECUTABLE)
   execute_process(COMMAND ${CPPCHECK_EXECUTABLE} --version
      OUTPUT_VARIABLE CPPCHECK_VERSION_STRING
      ERROR_VARIABLE CPPCHECK_VERSION_ERROR
      RESULT_VARIABLE CPPCHECK_VERSION_RESULT)
      
   get_filename_component(CPPCHECK_DIR ${CPPCHECK_EXECUTABLE} DIRECTORY)

   if(CPPCHECK_VERSION_RESULT)
      message(WARNING "Error getting CppCheck version : ${CPPCHECK_VERSION_ERROR}")
   else()
      string(REGEX REPLACE ".*Cppcheck ([0-9.]+).*" "\\1" CPPCHECK_VERSION "${CPPCHECK_VERSION_STRING}") 
      message(STATUS "Found CppCheck version : ${CPPCHECK_VERSION}")
      if(${CPPCHECK_VERSION} VERSION_LESS ${CPPCHECK_VERSION_MINIMAL})
         message(WARNING "Wrong CppCheck version : ${CPPCHECK_VERSION} found, ${CPPCHECK_VERSION_MINIMAL} minimal required")
      endif()
     endif()

   include(FindPackageHandleStandardArgs)
   FIND_PACKAGE_HANDLE_STANDARD_ARGS(CPPCHECK REQUIRED_VARS CPPCHECK_EXECUTABLE CPPCHECK_DIR
                                     VERSION_VAR CPPCHECK_VERSION)
                                     
endif()

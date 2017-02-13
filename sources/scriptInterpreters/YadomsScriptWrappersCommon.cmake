# Macros for setting up a script wrapper for yScriptApi
#
# Note that we use SWIG to generate wrappers.
# CMake macro for SWIG generates a target called _yScriptApiWrapper

INCLUDE(${SWIG_USE_FILE})

MACRO(SCRIPT_API_WRAPPER_SET_PARENT_INTERPRETER parentInterpreter)
   foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
       string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
       set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG}/scriptInterpreters/${parentInterpreter})
   endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
ENDMACRO()
   
MACRO(SCRIPT_API_WRAPPER_SOURCES targetLanguage)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_INCL_DIR})
   SET(SWIG_MODULE_yScriptApiWrapper_EXTRA_DEPS ${SHARED_COMMON_INCL_DIR}/shared/script/yScriptApi/IYScriptApi.h)
   SWIG_ADD_MODULE(yScriptApiWrapper ${targetLanguage} ${ARGN})
ENDMACRO()

MACRO(SCRIPT_API_WRAPPER_INCLDIR)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_INCL_DIR} ${ARGN})
ENDMACRO()

MACRO(SCRIPT_API_WRAPPER_LINK)
   SWIG_LINK_LIBRARIES(yScriptApiWrapper yadoms-shared ${ARGN})
ENDMACRO()

MACRO(SCRIPT_API_SOURCE_GROUP parentInterpreter)
   IF(MSVC OR XCODE)
      SET_PROPERTY(TARGET ${SWIG_MODULE_yScriptApiWrapper_REAL_NAME} PROPERTY FOLDER "scriptInterpreters/${parentInterpreter}/yScriptApiWrapper")
   ENDIF()
ENDMACRO()

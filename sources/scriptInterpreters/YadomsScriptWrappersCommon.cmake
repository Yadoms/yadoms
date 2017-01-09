# Macros for setting up a script wrapper for yScriptApi
#
# Note that we use SWIG to generate wrappers.
# CMake macro for SWIG generates a target called _yScriptApiWrapper

INCLUDE(${SWIG_USE_FILE})

MACRO(SET_PARENT_INTERPRETER interpreter)
   foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
       string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
       set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG}/scriptInterpreters/${interpreter})
   endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
ENDMACRO()
   
MACRO(SCRIPT_API_WRAPPER_SOURCES targetLanguage)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_INCL_DIR})
   SET(SWIG_MODULE_yScriptApiWrapper_EXTRA_DEPS ${SHARED_COMMON_INCL_DIR}/shared/script/yScriptApi/IYScriptApi.h)
   SWIG_ADD_MODULE(yScriptApiWrapper ${targetLanguage} ${ARGN})

   IF(MSVC OR XCODE)
      SET_PROPERTY(TARGET ${SWIG_MODULE_yScriptApiWrapper_REAL_NAME} PROPERTY FOLDER "scriptInterpreters/${targetLanguage}/yScriptApiWrapper")
   ENDIF()
ENDMACRO()

MACRO(SCRIPT_API_WRAPPER_INCLDIR)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${Poco_INCLUDE_DIRS} ${ARGN})
ENDMACRO()

MACRO(SCRIPT_API_WRAPPER_LINK)
   SWIG_LINK_LIBRARIES(yScriptApiWrapper yadoms-shared ${ARGN})
ENDMACRO()

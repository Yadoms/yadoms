# Macros for setting up a script wrapper for yScriptApi
#

INCLUDE(${SWIG_USE_FILE})
   

MACRO(SCRIPT_API_WRAPPER_SOURCES targetLanguage)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_INCL_DIR})
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

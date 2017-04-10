
# include common script interpreters definitions
include("../../YadomsScriptWrappersCommon.cmake")

set(YPYTHON27_PARENT_INTERPRETER yPython27)
set(YPYTHON27_TARGET_LANGUAGE python)


SCRIPT_API_WRAPPER_SET_PARENT_INTERPRETER(${YPYTHON27_PARENT_INTERPRETER})

#########################################
## Place here all the source files
#########################################
set(YPYTHON27_WRAPPER_SOURCES
   yScriptApiWrapper.i
   
	swigpyrun.h
     
   PythonLibInclude.h
   
   yScriptApiDoc.md
)
  

#########################################
## Active to wrap C++ code
#########################################
set_source_files_properties(yScriptApiWrapper.i PROPERTIES CPLUSPLUS ON)


#########################################
## Place here all additional include directories
## (by default yadoms-shared, boost and poco are included)
#########################################
set(YPYTHON27_WRAPPER_INCLDIR
   ${PYTHON_INCLUDE_DIRS}
   ${script_cpp_api_INCLUDE_DIR}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared, boost and poco are included)
#########################################
set(YPYTHON27_WRAPPER_LINK
   ${PYTHON_LIBRARIES}
   script_cpp_api
)

#########################################
## Place here all additional compiler flags
#########################################
# Special workaround for Python under Windows : When using the Windows installer version of Python, the debug library is not provided.
# We should use this workaround to make our application build in debug configuration.
if(WIN32 AND NOT PYTHON_USE_SOURCES)
   add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
endif()

if(PYTHON_USE_SOURCES)
   add_definitions(-DPYTHON_USE_SOURCES)
endif()

## Remove some warnings on generated code
if(MSVC)
   add_definitions( "/wd4706 /wd4701 /wd4459 /wd4456 /wd4702" )
endif()

#########################################
## Always leave these lines to configure script wrapper correctly
## even if some variables are empty
#########################################
SCRIPT_API_WRAPPER_SOURCES(${YPYTHON27_TARGET_LANGUAGE} ${YPYTHON27_WRAPPER_SOURCES})
SCRIPT_API_WRAPPER_LINK(${YPYTHON27_WRAPPER_LINK})
SCRIPT_API_WRAPPER_INCLDIR(${YPYTHON27_WRAPPER_INCLDIR} ${PROTOBUF_INCLUDE_DIRS})
SCRIPT_API_SOURCE_GROUP(${YPYTHON27_PARENT_INTERPRETER})

#########################################
## Add pre-build step to generate swigpyrun.h file
##
## Do not use PRE_BUILD option because it is working only with VisualStudio
## To make it work, just define a custom_command, and place 
## the output filename into the target sources to make it generated at build time
#########################################
add_custom_command(OUTPUT swigpyrun.h 
  COMMAND ${SWIG_EXECUTABLE} -${YPYTHON27_TARGET_LANGUAGE} -external-runtime ${CMAKE_CURRENT_SOURCE_DIR}/swigpyrun.h)
  
#########################################
## Add post-build step to deploy Swig-generated file
#########################################
SCRIPT_API_WRAPPER_POST_BUILD_COPY_FILE(${CMAKE_CURRENT_BINARY_DIR}/yScriptApiWrapper.py ${YPYTHON27_PARENT_INTERPRETER})
  
#########################################
## Add post-build step to deploy Api doc file
#########################################
SCRIPT_API_WRAPPER_POST_BUILD_COPY_FILE(${CMAKE_CURRENT_SOURCE_DIR}/yScriptApiDoc.md ${YPYTHON27_PARENT_INTERPRETER})


install(TARGETS _yScriptApiWrapper DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/yPython27)
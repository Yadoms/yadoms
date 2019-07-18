
# include common script interpreters definitions
include("../../YadomsScriptWrappersCommon.cmake")

set(YPYTHON3_PARENT_INTERPRETER yPython3)
set(YPYTHON3_TARGET_LANGUAGE python)


SCRIPT_API_WRAPPER_SET_PARENT_INTERPRETER(${YPYTHON3_PARENT_INTERPRETER})

#########################################
## Place here all the source files
#########################################
set(YPYTHON3_WRAPPER_SOURCES
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
set(YPYTHON3_WRAPPER_INCLDIR
   ${Python3_INCLUDE_DIRS}
   ${script_cpp_api_INCLUDE_DIR}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared, boost and poco are included)
#########################################
set(YPYTHON3_WRAPPER_LINK
   ${Python3_LIBRARIES}
   script_cpp_api
)

#########################################
## Place here all additional compiler flags
#########################################
# Special workaround for Python under Windows : When using the Windows installer version of Python, the debug library is not provided.
# We should use this workaround to make our application build in debug configuration.
if(WIN32 AND NOT PYTHON3_USE_SOURCES)
   add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
endif()

if(PYTHON3_USE_SOURCES)
   add_definitions(-DPYTHON3_USE_SOURCES)
endif()

## Remove some warnings on generated code
if(MSVC)
   add_definitions( "/wd4706 /wd4701 /wd4459 /wd4456 /wd4702" )
endif()

#########################################
## Always leave these lines to configure script wrapper correctly
## even if some variables are empty
#########################################
SCRIPT_API_WRAPPER_SOURCES(yScriptApiWrapper3 ${YPYTHON3_TARGET_LANGUAGE} ${YPYTHON3_WRAPPER_SOURCES})
SCRIPT_API_WRAPPER_LINK(yScriptApiWrapper3 ${YPYTHON3_WRAPPER_LINK})
SCRIPT_API_WRAPPER_INCLDIR(yScriptApiWrapper3 ${YPYTHON3_WRAPPER_INCLDIR} ${PROTOBUF_INCLUDE_DIRS})
SCRIPT_API_SOURCE_GROUP(${YPYTHON3_PARENT_INTERPRETER})

#########################################
## Add pre-build step to generate swigpyrun.h file
##
## Do not use PRE_BUILD option because it is working only with VisualStudio
## To make it work, just define a custom_command, and place 
## the output filename into the target sources to make it generated at build time
#########################################
add_custom_command(OUTPUT swigpyrun.h 
  COMMAND ${SWIG_EXECUTABLE} -${YPYTHON3_TARGET_LANGUAGE} -external-runtime ${CMAKE_CURRENT_SOURCE_DIR}/swigpyrun.h)
  
#########################################
## Add post-build step to deploy Swig-generated file
#########################################
SCRIPT_API_WRAPPER_POST_BUILD_COPY_FILE(yScriptApiWrapper3 ${CMAKE_CURRENT_BINARY_DIR}/yScriptApiWrapper3.py ${YPYTHON3_PARENT_INTERPRETER})
  
#########################################
## Add post-build step to deploy Api doc file
#########################################
SCRIPT_API_WRAPPER_POST_BUILD_COPY_FILE(yScriptApiWrapper3 ${CMAKE_CURRENT_SOURCE_DIR}/yScriptApiDoc.md ${YPYTHON3_PARENT_INTERPRETER})


install(TARGETS yScriptApiWrapper3 DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/yPython3)

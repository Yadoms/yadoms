
# include common script interpreters definitions
include("../../YadomsScriptWrappersCommon.cmake")

#########################################
## Place here all the source files
#########################################
set(PYTHON_WRAPPER_SOURCES
   yScriptApiWrapper.i

   stdafx.h
   stdafx.cpp
   
	swigpyrun.h
     
   PythonLibInclude.h
   yScriptApiInstance.h
   yScriptApiInstance.cpp
   yScriptApiImplementation.h
   yScriptApiImplementation.cpp
   
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
set(PYTHON_WRAPPER_INCLDIR
   ${PYTHON_INCLUDE_DIRS}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared, boost and poco are included)
#########################################
set(PYTHON_WRAPPER_LINK
   ${PYTHON_LIBRARIES}
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
SCRIPT_API_WRAPPER_SOURCES(python ${PYTHON_WRAPPER_SOURCES})
SCRIPT_API_WRAPPER_INCLDIR(${PYTHON_WRAPPER_INCLDIR} ${PROTOBUF_INCLUDE_DIRS} ${messages_GENERATED_DIR})
SCRIPT_API_WRAPPER_LINK(${PYTHON_WRAPPER_LINK} messages)

#########################################
## Add pre-build step to generate swigpyrun.h file
##
## Do not use PRE_BUILD option because it is working only with VisualStudio
## To make it work, just define a custom_command, and place 
## the output filename into the target sources to make it generated at build time
#########################################
add_custom_command(OUTPUT swigpyrun.h 
  COMMAND ${SWIG_EXECUTABLE} -python -external-runtime ${CMAKE_CURRENT_SOURCE_DIR}/swigpyrun.h)
  

#########################################
## Add post-build step to deploy Swig-generated file
#########################################
add_custom_command(TARGET _yScriptApiWrapper POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/yScriptApiWrapper.py $<TARGET_FILE_DIR:_yScriptApiWrapper>/yScriptApiWrapper.py)
  

#########################################
## Add post-build step to deploy Api doc file
#########################################
add_custom_command(TARGET _yScriptApiWrapper POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/yScriptApiDoc.md $<TARGET_FILE_DIR:_yScriptApiWrapper>/yScriptApiDoc.md)


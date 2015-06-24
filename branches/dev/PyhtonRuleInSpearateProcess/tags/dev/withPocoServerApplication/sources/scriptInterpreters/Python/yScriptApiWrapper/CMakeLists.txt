
# include common script interpreters definitions
include("../../YadomsScriptWrappersCommon.cmake")

#########################################
## Place here all the source files
#########################################
set(PYTHON_WRAPPER_SOURCES
   yScriptApiWrapper.i

   stdafx.h
   stdafx.cpp
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
# Special workaround for Python under Windows : When using the Windows installer version of Python, the debug library is not provide.
# We should the use this workaround to make our application build in debug configuration.
if(WIN32 AND NOT PYTHON_USE_SOURCES)
   add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
endif()

## Remove some warnings on generated code
if(MSVC)
   add_definitions( "/wd4706 /wd4701" )
endif()

#########################################
## Always leave these lines to configure script wrapper correctly
## even if some variables are empty
#########################################
SCRIPT_API_WRAPPER_SOURCES(python ${PYTHON_WRAPPER_SOURCES})
SCRIPT_API_WRAPPER_INCLDIR(${PYTHON_WRAPPER_INCLDIR})
SCRIPT_API_WRAPPER_LINK(${PYTHON_WRAPPER_LINK})



#########################################
## Add post-build step to deploy Swig-generated file
#########################################
add_custom_command(TARGET _yScriptApiWrapper POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/yScriptApiWrapper.py $<TARGET_FILE_DIR:_yScriptApiWrapper>/yScriptApiWrapper.py)


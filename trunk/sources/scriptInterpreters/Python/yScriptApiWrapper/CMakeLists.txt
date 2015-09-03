
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
     
   ../Messages.hpp
   PythonLibInclude.h
   yScriptApiInstance.h
   yScriptApiInstance.cpp
   yScriptApiImplementation.h
   yScriptApiImplementation.cpp
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
## Add pre-build step to generate swigpyrun.h file
##
## Do not use PRE_BUILD option because it is working only with VisualStudio
## To make it work, just define a custom_command, and place 
## the output filename into the target sources to make it generated at build time
#########################################
add_custom_command(OUTPUT swigpyrun.h 
  COMMAND ${SWIG_EXECUTABLE} -python -external-runtime ${CMAKE_CURRENT_SOURCE_DIR}/swigpyrun.h)

#########################################
## Add pre-build step to generate Python API documentation
##
## To generate API documentation, these steps are necessary :
## - Doxygen the C++ API to XML format :
##   - inputs are :
##      - shared/shared/script/yScriptApi/IYScriptApi.h 
##      - doc/Doxyfile
##   - outputs are :
##      - doc/out/IYScriptApiDoc.xml use to generate Python autodoc
##      - doc/out/html : API documentation in html format
## - Extract Python documentation from XML using doxy2swig.py
##   - input is doc/out/IYScriptApiDoc.xml
##   - output is doc/out/IYScriptApiDoc.i
##
## Do not use PRE_BUILD option because it is working only with VisualStudio
## To make it work, just define a custom_command, and place 
## the output filename into the target sources to make it generated at build time
#########################################
# if(DOXYGEN_FOUND)
# add_custom_command(OUTPUT doc/out/xml/IYScriptApiDoc.xml
   # COMMAND ${DOXYGEN_EXECUTABLE}) doc/Doxyfile
# else()
   # message("doxygen not found : YScriptApi documentation will not be generated") ## TODO à tester
# endif()
  

#########################################
## Add post-build step to deploy Swig-generated file
#########################################
add_custom_command(TARGET _yScriptApiWrapper POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/yScriptApiWrapper.py $<TARGET_FILE_DIR:_yScriptApiWrapper>/yScriptApiWrapper.py)



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
   ${Python2_INCLUDE_DIRS}
   ${script_cpp_api_INCLUDE_DIR}
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared, boost and poco are included)
#########################################
set(YPYTHON27_WRAPPER_LINK
   ${Python2_LIBRARIES}
   script_cpp_api
)

#########################################
## Place here all additional compiler flags
#########################################
# Special workaround for Python under Windows : When using the Windows installer version of Python, the debug library is not provided.
# We should use this workaround to make our application build in debug configuration.
if(WIN32 AND NOT PYTHON27_USE_SOURCES)
   add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
endif()

if(PYTHON27_USE_SOURCES)
   add_definitions(-DPYTHON27_USE_SOURCES)
endif()

## Remove some warnings on generated code
if(MSVC)
   add_definitions( "/wd4706 /wd4701 /wd4459 /wd4456 /wd4702" )
endif()

##specific linker lib for osxcross (cross build for Darwin)
if (CMAKE_SYSTEM_NAME MATCHES Darwin)
	#check osxcross cross compilation
	#in taht case, we must use 
	find_package(Iconv)
	if(Iconv_FOUND)
			set(YPYTHON27_WRAPPER_LINK ${YPYTHON27_WRAPPER_LINK} ${Iconv_LIBRARIES})
	else(Iconv_FOUND)
			message(FATAL_ERROR "libiconv not FOUND")
	endif()
	find_package(Intl)
	if(Intl_FOUND)
			set(YPYTHON27_WRAPPER_LINK ${YPYTHON27_WRAPPER_LINK} ${Intl_LIBRARIES})
	else(Intl_FOUND)
			message(FATAL_ERROR "libintl not FOUND")
	endif()
endif()


#########################################
## Always leave these lines to configure script wrapper correctly
## even if some variables are empty
#########################################
SCRIPT_API_WRAPPER_SOURCES(yScriptApiWrapper27 ${YPYTHON27_TARGET_LANGUAGE} ${YPYTHON27_WRAPPER_SOURCES})
SCRIPT_API_WRAPPER_LINK(yScriptApiWrapper27 ${YPYTHON27_WRAPPER_LINK})
SCRIPT_API_WRAPPER_INCLDIR(yScriptApiWrapper27 ${YPYTHON27_WRAPPER_INCLDIR} ${PROTOBUF_INCLUDE_DIRS})
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
SCRIPT_API_WRAPPER_POST_BUILD_COPY_FILE(yScriptApiWrapper27 ${CMAKE_CURRENT_BINARY_DIR}/yScriptApiWrapper27.py ${YPYTHON27_PARENT_INTERPRETER})
  
#########################################
## Add post-build step to deploy Api doc file
#########################################
SCRIPT_API_WRAPPER_POST_BUILD_COPY_FILE(yScriptApiWrapper27 ${CMAKE_CURRENT_SOURCE_DIR}/yScriptApiDoc.md ${YPYTHON27_PARENT_INTERPRETER})


install(TARGETS yScriptApiWrapper27 DESTINATION ${INSTALL_BINDIR}/scriptInterpreters/yPython27)

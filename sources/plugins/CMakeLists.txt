#This file automatically search and add all plugins found from all subdirectories

set(PLUGIN_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)

#This macro list all subdirectories
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

#list all subdirectories from here
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

#foreach subdirectory, search for CMakeList.txt
#if there is one, then it is a plugin
FOREACH(subdir ${SUBDIRS})
	IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
		message(STATUS "Found plugin " ${subdir})
		ADD_SUBDIRECTORY(${subdir})
	ENDIF()
ENDFOREACH()

#share the plugin list with main cmake script
set(PLUGINLIST
   ${PLUGINLIST}
   PARENT_SCOPE)
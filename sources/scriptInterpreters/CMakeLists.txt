#This file automatically search and add all script interpreters found from all subdirectories

# remove warning
cmake_policy(SET CMP0074 NEW)

set(SWIG_VERSION_REQUIRED 3.0.5)
include(findSwig.cmake)
  

if(SWIG_FOUND)

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
   #if there is one, then it is a script interpreters
   FOREACH(subdir ${SUBDIRS})
      IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
         message(STATUS "Found script interpreter " ${subdir})
         ADD_SUBDIRECTORY(${subdir})
      ENDIF()
   ENDFOREACH()

   set(SCRIPTINTERPRETERSLIST
      ${SCRIPTINTERPRETERSLIST}
      PARENT_SCOPE)   
   
else()
   message("SWIG not found : no script interpreter will be available")
endif()
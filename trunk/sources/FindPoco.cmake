# find the poco libraries
#
#Input
#  POCO_ROOT should be defined, either in env vars, or in CMakeList.txt
#	set the POCO_LINK_STATIC to link statically, else if not defined, shared link is used
#
#Output
# 	${Poco_INCLUDE_DIRS}		contains all the include directories for Poco
# 	${Poco_FOUND_LIBS}		contains all the library files
# 	${Poco_FOUND_BINS}		contains all the binary files (only if shared)





SET(POCO_INCLUDE_PATH_DESCRIPTION "top-level directory containing the poco include directories. E.g /usr/local/include/poco-1.2.1 or c:\\poco\\include\\poco-1.2.1")
SET(POCO_INCLUDE_DIR_MESSAGE "Set the Poco_INCLUDE_DIR cmake cache entry to the ${POCO_INCLUDE_PATH_DESCRIPTION}")
SET(POCO_LIBRARY_PATH_DESCRIPTION "top-level directory containing the poco libraries.")
SET(POCO_LIBRARY_DIR_MESSAGE "Set the Poco_LIBRARY_DIR cmake cache entry to the ${POCO_LIBRARY_PATH_DESCRIPTION}")

	
SET(POCO_DIR_SEARCH $ENV{POCO_ROOT})
IF(NOT POCO_DIR_SEARCH)
	SET(POCO_DIR_SEARCH ${POCO_ROOT})
ENDIF()
IF(POCO_DIR_SEARCH)
  FILE(TO_CMAKE_PATH ${POCO_DIR_SEARCH} POCO_DIR_SEARCH)
ENDIF(POCO_DIR_SEARCH)

#
# Look for an installation.
#
FIND_PATH(Poco_INCLUDE_DIR NAMES Foundation/include/Poco/AbstractCache.h PATH_SUFFIXES PATHS

  # Look in other places.
  ${POCO_DIR_SEARCH}

  # Help the user find it if we cannot.
  DOC "The ${POCO_INCLUDE_DIR_MESSAGE}"
)




#
# Look for an foundation lib directory
#

# Add in some path suffixes. These will have to be updated whenever a new Poco version comes out.
SET(SUFFIX_FOR_LIBRARY_PATH
 lib
 lib/Linux/i686
 lib/Linux/x86_64
 lib/Linux/ARM
 lib/Darwin/x86_64
 lib/Linux/armv6l
 lib/Linux/armv7l
)


SET(POCO_FOUNDATION PocoFoundation)
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
	SET(POCO_FOUNDATION PocoFoundationd)
endif()		

FIND_LIBRARY(Poco_LIB_DIR NAMES ${POCO_FOUNDATION}  PATH_SUFFIXES ${SUFFIX_FOR_LIBRARY_PATH} PATHS

 # Look in other places.
 ${Poco_INCLUDE_DIR}
 ${POCO_DIR_SEARCH}

 # Help the user find it if we cannot.
 DOC "The ${POCO_LIBRARY_PATH_DESCRIPTION}"
)
GET_FILENAME_COMPONENT(Poco_LIB_DIR ${Poco_LIB_DIR} PATH)	


#
# Look for an foundation bin directory
#

# Add in some path suffixes. These will have to be updated whenever a new Poco version comes out.
SET(SUFFIX_FOR_LIBRARY_BIN_PATH
 bin
 bin/Linux/i686
)
if(WIN32)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
elseif(APPLE)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".so")
else()
	SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
endif()
	

FIND_LIBRARY(Poco_BIN_DIR NAMES ${POCO_FOUNDATION}  PATH_SUFFIXES ${SUFFIX_FOR_LIBRARY_BIN_PATH} PATHS

 # Look in other places.
 ${Poco_INCLUDE_DIR}
 ${POCO_DIR_SEARCH}

 # Help the user find it if we cannot.
 DOC "The ${POCO_LIBRARY_PATH_DESCRIPTION}"
)
GET_FILENAME_COMPONENT(Poco_BIN_DIR ${Poco_BIN_DIR} PATH)	


# Assume we didn't find it.
SET(Poco_FOUND 0)

# Now try to get the include and library path.
IF(Poco_INCLUDE_DIR)

  #create the Poco_INCLUDE_DIRS which contains each library include directory
  IF(EXISTS "${Poco_INCLUDE_DIR}")
    FOREACH (POCOLIB ${POCO_LIBS})
	 
		#include directory for current lib (ensure not starting with Poco
		SET(POCOLIB_SUBDIR ${POCOLIB})
   	string(REGEX REPLACE "^Poco" "" POCOLIB_SUBDIR ${POCOLIB_SUBDIR})
		
      SET(CURRENTINCLUDE_LIB ${Poco_INCLUDE_DIR}/${POCOLIB_SUBDIR}/include)
      IF(EXISTS "${CURRENTINCLUDE_LIB}")
        SET(Poco_INCLUDE_DIRS ${Poco_INCLUDE_DIRS} ${CURRENTINCLUDE_LIB})
      ELSE()
        message(FATAL_ERROR "ERROR: cannot find ${POCOLIB} include dir. Should be : ${CURRENTINCLUDE_LIB}")
      ENDIF()
		
		#find the lib to link with
		if(WIN32)
			SET(CURRENT_DEBUG_LIB ${Poco_LIB_DIR}/${POCOLIB}d.lib)
			SET(CURRENT_RELEASE_LIB ${Poco_LIB_DIR}/${POCOLIB}.lib)
		elseif(APPLE)
			SET(CURRENT_DEBUG_LIB ${Poco_LIB_DIR}/lib${POCOLIB}d.dylib)
			SET(CURRENT_RELEASE_LIB ${Poco_LIB_DIR}/lib${POCOLIB}.dylib)
		else()
			SET(CURRENT_DEBUG_LIB ${Poco_LIB_DIR}/lib${POCOLIB}d.so)
			SET(CURRENT_RELEASE_LIB ${Poco_LIB_DIR}/lib${POCOLIB}.so)
		endif()

		IF(EXISTS "${CURRENT_DEBUG_LIB}")
		  SET(Poco_FOUND_LIBS ${Poco_FOUND_LIBS} "debug" ${CURRENT_DEBUG_LIB})
		ELSE()
		  message(FATAL_ERROR "ERROR: cannot find ${POCOLIB} DEBUG library file. Should be : ${CURRENT_DEBUG_LIB}")
		ENDIF()
		
		IF(EXISTS "${CURRENT_RELEASE_LIB}")
		  SET(Poco_FOUND_LIBS ${Poco_FOUND_LIBS} "optimized" ${CURRENT_RELEASE_LIB})
		ELSE()
		  message(FATAL_ERROR "ERROR: cannot find ${POCOLIB} RELEASE library file. Should be : ${CURRENT_RELEASE_LIB}")
		ENDIF()
		
				
		if(NOT POCO_LINK_STATIC)
			#link=shared
			if(WIN32)
				SET(CURRENT_DEBUG_BIN ${Poco_BIN_DIR}/${POCOLIB}d.dll)
				SET(CURRENT_RELEASE_BIN ${Poco_BIN_DIR}/${POCOLIB}.dll)
				IF(EXISTS "${CURRENT_DEBUG_BIN}")
				  SET(Poco_FOUND_BINS ${Poco_FOUND_BINS} "debug" ${CURRENT_DEBUG_BIN})
				ELSE()
				  message(FATAL_ERROR "ERROR: cannot find ${CURRENT_DEBUG_BIN} binary file. Should be : ${CURRENT_DEBUG_BIN}")
				ENDIF()
				IF(EXISTS "${CURRENT_RELEASE_BIN}")
				  SET(Poco_FOUND_BINS ${Poco_FOUND_BINS} "optimized" ${CURRENT_RELEASE_BIN})
				ELSE()
				  message(FATAL_ERROR "ERROR: cannot find ${CURRENT_RELEASE_BIN} binary file. Should be : ${CURRENT_RELEASE_BIN}")
				ENDIF()
			endif()

		endif(NOT POCO_LINK_STATIC)
    ENDFOREACH(POCOLIB)

    SET(Poco_FOUND 1)
  ENDIF(EXISTS "${Poco_INCLUDE_DIR}")

ENDIF(Poco_INCLUDE_DIR)



IF(NOT Poco_FOUND)
  IF(NOT Poco_FIND_QUIETLY)
    MESSAGE(STATUS "Poco was not found. ${POCO_DIR_MESSAGE}")
  ELSE(NOT Poco_FIND_QUIETLY)
    IF(Poco_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Poco was not found. ${POCO_DIR_MESSAGE}")
    ENDIF(Poco_FIND_REQUIRED)
  ENDIF(NOT Poco_FIND_QUIETLY)
else(NOT Poco_FOUND)

  if(NOT Poco_FIND_QUIETLY)
      message(STATUS "Found the following Poco libraries:")
	  foreach( COMPONENT  ${POCO_LIBS} )
		 message (STATUS "  ${COMPONENT}")
	  endforeach()
  endif()

ENDIF(NOT Poco_FOUND)

mark_as_advanced(Poco_FOUND, Poco_INCLUDE_DIRS, Poco_FOUND_LIBS, Poco_FOUND_BINS)




MACRO(POST_BUILD_COPY_POCO target)

 ##################################################################################################
   ## Post build commands
   ##################################################################################################

   #create lib lists, one for debug libs and another for optimized (release)
   # Poco_FOUND_BINS is a list which contains something like this:
   #
   #	debug							(<- define that next list entry if for debug mode)
   #	path/to/debug/Poco...d.dll)
   #	optimize                        (<- define that next list entry if for release mode)
   #	path/to/debug/Poco...dll)
   #	...
   #
   # In some configurations, the list contains only lib file path without (debug/optimize)
   # So if debug and optimize or not found, then just add library to debug or optimized list
   #
   set(TYPEISKNOWN OFF)
   set(TYPEISDEBUG OFF)
   FOREACH (POCOLIBRARY ${Poco_FOUND_BINS})
      if(TYPEISKNOWN)
         if(TYPEISDEBUG)
            list(APPEND POCO_LIBS_DEBUG ${POCOLIBRARY})
         else()
            list(APPEND POCO_LIBS_OPTIMIZED ${POCOLIBRARY})
         endif()
         set(TYPEISKNOWN OFF)
      else()
         if(${POCOLIBRARY} MATCHES "debug") 
            set(TYPEISKNOWN ON)
            set(TYPEISDEBUG ON)
         elseif(${POCOLIBRARY} MATCHES "optimized") 
            set(TYPEISKNOWN ON)
            set(TYPEISDEBUG OFF)
         else()
            list(APPEND POCO_LIBS_DEBUG ${POCOLIBRARY})
         endif()
      endif()
   ENDFOREACH(POCOLIBRARY)


   #Take each boost lib file, find its dll and add it to a post build command
   if(NOT POCO_LINK_STATIC)
	
      MESSAGE(STATUS "Add Poco libraries to be copied as postbuild")
      FOREACH (POCOLIBRARY ${POCO_LIBS_DEBUG})
            #create post build command
            add_custom_command(TARGET ${target} POST_BUILD        	
               COMMAND ${CMAKE_COMMAND} -E copy_if_different  	
                  "${POCOLIBRARY}"     							
                  $<TARGET_FILE_DIR:${target}>)   				
      ENDFOREACH(POCOLIBRARY)
		
      FOREACH (POCOLIBRARY ${POCO_LIBS_RELEASE})
            add_custom_command(TARGET ${target} POST_BUILD        	
               COMMAND ${CMAKE_COMMAND} -E copy_if_different  	
                  "${POCOLIBRARY}"     							
                  $<TARGET_FILE_DIR:${target}>)   				
      ENDFOREACH(POCOLIBRARY)		
   endif()		


	
ENDMACRO()

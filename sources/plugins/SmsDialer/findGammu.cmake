# Gammu library Finder
# Inputs :
#   - GAMMU_ROOT : Gammu root directory
# Outputs : this module defines :
#   - GAMMU_FOUND, true if Gammu was found
#   - GAMMU_INCLUDE_DIRS, where to find gammu.h
#   - GAMMU_LIBRARIES, where to find libraries needed to build with Gammu

if(MSVC)
  include (SelectLibraryConfigurations)

  # Include directory
  set(GAMMU_PKG_INCLUDE_DIRS ${GAMMU_ROOT}/build/include)
  
  # Libraries
  set(GAMMU_PKG_LIBRARY_DIR_DEBUG ${GAMMU_ROOT}/build/libgammu/Debug)
  set(GAMMU_PKG_LIBRARY_DIR_RELEASE ${GAMMU_ROOT}/build/libgammu/Release)
  set(GAMMU_PKG_LIBRARIES Gammu.lib)
  
  find_library(GAMMU_LIBRARY_DEBUG NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIR_DEBUG})
  find_library(GAMMU_LIBRARY_RELEASE NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIR_RELEASE})
  select_library_configurations(GAMMU)

  # Additional libraries (required by Gammu)
  set(GAMMU_PKG_HELPER_DIR_DEBUG ${GAMMU_ROOT}/build/helper/Debug)
  set(GAMMU_PKG_HELPER_DIR_RELEASE ${GAMMU_ROOT}/build/helper/Release)
  # - string.lib
  set(GAMMU_PKG_STRING_LIBRARIES string.lib)
  find_library(GAMMU_STRING_LIBRARY_DEBUG NAMES ${GAMMU_PKG_STRING_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_DEBUG})
  find_library(GAMMU_STRING_LIBRARY_RELEASE NAMES ${GAMMU_PKG_STRING_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_RELEASE})
  select_library_configurations(GAMMU_STRING)
  if(GAMMU_STRING_FOUND)
	 set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_ADDITIONAL_LIBRARIES} ${GAMMU_STRING_LIBRARIES})
  endif(GAMMU_STRING_FOUND)
  
  # - win32dirent.lib
  set(GAMMU_PKG_WIN32DIRENT_LIBRARIES win32dirent.lib)
  find_library(GAMMU_WIN32DIRENT_LIBRARY_DEBUG NAMES ${GAMMU_PKG_WIN32DIRENT_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_DEBUG})
  find_library(GAMMU_WIN32DIRENT_LIBRARY_RELEASE NAMES ${GAMMU_PKG_WIN32DIRENT_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_RELEASE})
  select_library_configurations(GAMMU_WIN32DIRENT)
  
  if(GAMMU_WIN32DIRENT_FOUND)
	 set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_ADDITIONAL_LIBRARIES} ${GAMMU_WIN32DIRENT_LIBRARIES})
  endif(GAMMU_WIN32DIRENT_FOUND)

  
else()
	set(GAMMU_PKG_INCLUDE_DIRS 
		${GAMMU_ROOT}/include
		${GAMMU_ROOT}/build/include
		${GAMMU_ROOT}/include/gammu
		${GAMMU_ROOT}/build/include/gammu
	)
	
	set(GAMMU_PKG_LIBRARY_DIRS  
		${GAMMU_ROOT}/build/libgammu 
		${GAMMU_ROOT}/lib
	)
	
	set(GAMMU_PKG_LIBRARIES libGammu.a)
	if (CMAKE_SYSTEM_NAME MATCHES Darwin)
		#check osxcross cross compilation
		#in taht case, we must use 
		if(GAMMU_LINK_DEPS)
			set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_ADDITIONAL_LIBRARIES} ${GAMMU_LINK_DEPS})
		else()
			find_package(Iconv)
			if(Iconv_FOUND)
				set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_ADDITIONAL_LIBRARIES} ${Iconv_LIBRARIES})
			else(Iconv_FOUND)
				message(FATAL_ERROR "libiconv not FOUND")
			endif()
			find_package(Intl)
			if(Intl_FOUND)
				set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_ADDITIONAL_LIBRARIES} ${Intl_LIBRARIES})
			else(Intl_FOUND)
				message(FATAL_ERROR "libintl not FOUND")
			endif()
		endif()
	else()
	endif()

	find_library(GAMMU_LIBRARIES NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIRS} NO_DEFAULT_PATH)
  
endif()


if(GAMMU_LIBRARIES)

  set(GAMMU_INCLUDE_DIRS ${GAMMU_PKG_INCLUDE_DIRS})

  # Add additional libraries
  set(GAMMU_LIBRARIES ${GAMMU_LIBRARIES} ${GAMMU_ADDITIONAL_LIBRARIES})

  # handle the QUIETLY and REQUIRED arguments and set GAMMU_FOUND to TRUE if 
  # all listed variables are TRUE
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Gammu DEFAULT_MSG GAMMU_INCLUDE_DIRS GAMMU_LIBRARIES)
  
endif()




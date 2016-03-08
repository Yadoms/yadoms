# Gammu library Finder
# Inputs :
#   - GAMMU_ROOT : Gammu root directory
# Outputs : this module defines :
#   - GAMMU_FOUND, true if Gammu was found
#   - GAMMU_INCLUDE_DIRS, where to find gammu.h
#   - GAMMU_LIBRARIES, where to find libraries needed to build with Gammu

include (SelectLibraryConfigurations)

if(GAMMU_INCLUDE_DIRS AND GAMMU_LIBRARIES)
    # Already in cache
    set(Gammu_FIND_QUIETLY TRUE)
endif(GAMMU_INCLUDE_DIRS AND GAMMU_LIBRARIES)

if(MSVC)
   # Use GAMMU_ROOT defined in CMakeListsUserConfig.txt
   
   # Include directory
   set(GAMMU_PKG_INCLUDE_DIRS ${GAMMU_ROOT}/include)
   
   # Libraries
   set(GAMMU_PKG_LIBRARY_DIR_DEBUG ${GAMMU_ROOT}/libgammu/Debug)
   set(GAMMU_PKG_LIBRARY_DIR_RELEASE ${GAMMU_ROOT}/libgammu/Release)
   set(GAMMU_PKG_LIBRARIES Gammu.lib)
   
   find_library(GAMMU_LIBRARY_DEBUG NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIR_DEBUG})
   find_library(GAMMU_LIBRARY_RELEASE NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIR_RELEASE})
   select_library_configurations(GAMMU)

   # Additional libraries (required by Gammu)
   set(GAMMU_PKG_HELPER_DIR_DEBUG ${GAMMU_ROOT}/helper/Debug)
   set(GAMMU_PKG_HELPER_DIR_RELEASE ${GAMMU_ROOT}/helper/Release)
   # - string.lib
   set(GAMMU_PKG_STRING_LIBRARIES string.lib)
   find_library(GAMMU_STRING_LIBRARY_DEBUG NAMES ${GAMMU_PKG_STRING_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_DEBUG})
   find_library(GAMMU_STRING_LIBRARY_RELEASE NAMES ${GAMMU_PKG_STRING_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_RELEASE})
   select_library_configurations(GAMMU_STRING)
   # - win32dirent.lib
   set(GAMMU_PKG_WIN32DIRENT_LIBRARIES win32dirent.lib)
   find_library(GAMMU_WIN32DIRENT_LIBRARY_DEBUG NAMES ${GAMMU_PKG_WIN32DIRENT_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_DEBUG})
   find_library(GAMMU_WIN32DIRENT_LIBRARY_RELEASE NAMES ${GAMMU_PKG_WIN32DIRENT_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_RELEASE})
   select_library_configurations(GAMMU_WIN32DIRENT)
   
   set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_STRING_LIBRARIES} ${GAMMU_WIN32DIRENT_LIBRARIES})

endif()

if(GAMMU_USE_PKGCONFIG AND CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_COMPILER_IS_RASPBERRY_CROSS_COMPILER)

   # Use pkg-config to get the directories and then use these values
   # in the FIND_PATH() and FIND_LIBRARY() calls
   # This line will define these variables :
   #   - GAMMU_PKG_LIBRARIES : only the libraries
   #   - GAMMU_PKG_LIBRARY_DIRS : the paths of the libraries
   #   - GAMMU_PKG_LDFLAGS : all required linker flags
   #   - GAMMU_PKG_LDFLAGS_OTHER : all other linker flags
   #   - GAMMU_PKG_INCLUDE_DIRS : the '-I' preprocessor flags
   #   - GAMMU_PKG_CFLAGS : all required cflags
   #   - GAMMU_PKG_CFLAGS_OTHER : the other compiler flags
   find_package(PkgConfig)
   pkg_check_modules(GAMMU_PKG gammu)
   
   find_library(GAMMU_LIBRARIES NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIRS})
   
else(GAMMU_USE_PKGCONFIG AND CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_COMPILER_IS_RASPBERRY_CROSS_COMPILER)
   # Do not use PkgConfig as PkgConfig will return host libs
   # Use GAMMU_ROOT defined in CMakeListsUserConfig.txt
   
   if(APPLE)
      set(GAMMU_PKG_INCLUDE_DIRS ${GAMMU_ROOT}/build/include/gammu)
      set(GAMMU_PKG_LIBRARY_DIRS  ${GAMMU_ROOT}/build/lib)
      set(GAMMU_PKG_LIBRARIES libGammu.dylib)
   elseif(UNIX)
      set(GAMMU_PKG_INCLUDE_DIRS ${GAMMU_ROOT}/build/include)
      set(GAMMU_PKG_LIBRARY_DIRS  ${GAMMU_ROOT}/build/libgammu)
      set(GAMMU_PKG_LIBRARIES libGammu.so)
   endif()
   
   find_library(GAMMU_LIBRARIES NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIRS} NO_DEFAULT_PATH)
   
endif(GAMMU_USE_PKGCONFIG AND CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_COMPILER_IS_RASPBERRY_CROSS_COMPILER)

find_path(GAMMU_INCLUDE_DIRS NAMES gammu.h
   PATHS
   ${GAMMU_PKG_INCLUDE_DIRS})

# Add additional libraries
set(GAMMU_LIBRARIES ${GAMMU_LIBRARIES} ${GAMMU_ADDITIONAL_LIBRARIES})

# handle the QUIETLY and REQUIRED arguments and set GAMMU_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gammu DEFAULT_MSG GAMMU_INCLUDE_DIRS GAMMU_LIBRARIES)

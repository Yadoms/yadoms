# Gammu library Finder
# Inputs :
#   - GAMMU_ROOT : Gammu root directory
# Outputs : this module defines :
#   - GAMMU_FOUND, true if Gammu was found
#   - GAMMU_INCLUDE_DIRS, where to find gammu.h
#   - GAMMU_LIBRARIES, where to find libraries needed to build with Gammu

if(NOT APPLE)

   include (SelectLibraryConfigurations)

   if(MSVC)
      # Use GAMMU_ROOT defined in CMakeListsUserConfig.txt
      
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
      # - win32dirent.lib
      set(GAMMU_PKG_WIN32DIRENT_LIBRARIES win32dirent.lib)
      find_library(GAMMU_WIN32DIRENT_LIBRARY_DEBUG NAMES ${GAMMU_PKG_WIN32DIRENT_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_DEBUG})
      find_library(GAMMU_WIN32DIRENT_LIBRARY_RELEASE NAMES ${GAMMU_PKG_WIN32DIRENT_LIBRARIES} PATHS ${GAMMU_PKG_HELPER_DIR_RELEASE})
      select_library_configurations(GAMMU_WIN32DIRENT)
      
      set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_STRING_LIBRARIES} ${GAMMU_WIN32DIRENT_LIBRARIES})
      
   else()
      set(GAMMU_PKG_INCLUDE_DIRS ${GAMMU_ROOT}/build/include)
      set(GAMMU_PKG_LIBRARY_DIRS  ${GAMMU_ROOT}/build/libgammu)
      set(GAMMU_PKG_LIBRARIES libGammu.a)
      
      set(GAMMU_ADDITIONAL_LIBRARIES ${GAMMU_STRING_LIBRARIES} ${GAMMU_WIN32DIRENT_LIBRARIES} usb-1.0 bluetooth glib-2.0)
         
      find_library(GAMMU_LIBRARIES NAMES ${GAMMU_PKG_LIBRARIES} PATHS ${GAMMU_PKG_LIBRARY_DIRS} NO_DEFAULT_PATH)
      
   endif()

   
   if(GAMMU_FOUND)

      set(GAMMU_INCLUDE_DIRS ${GAMMU_PKG_INCLUDE_DIRS})

      # Add additional libraries
      set(GAMMU_LIBRARIES ${GAMMU_LIBRARIES} ${GAMMU_ADDITIONAL_LIBRARIES})

      # handle the QUIETLY and REQUIRED arguments and set GAMMU_FOUND to TRUE if 
      # all listed variables are TRUE
      include(FindPackageHandleStandardArgs)
      find_package_handle_standard_args(Gammu DEFAULT_MSG GAMMU_INCLUDE_DIRS GAMMU_LIBRARIES)
      
   endif()

endif()


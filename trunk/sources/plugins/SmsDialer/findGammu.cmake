# Gammu library Finder
# This module defines
#  GAMMU_FOUND, true if Gammu was found
#  GAMMU_INCLUDE_DIRS, where to find gammu.h
#  GAMMU_LIBRARIES, where to find libraries needed to build with Gammu

include (CMakeListsUserConfig.txt OPTIONAL)

if(GAMMU_INCLUDE_DIRS AND GAMMU_LIBRARIES)
    # Already in cache
    set(Gammu_FIND_QUIETLY TRUE)
endif(GAMMU_INCLUDE_DIRS AND GAMMU_LIBRARIES)

if(WIN32)
   # Use GAMMU_ROOT defined in CMakeListsUserConfig.txt
   set(GAMMU_PKG_INCLUDE_DIRS ${GAMMU_ROOT}/include)
   set(GAMMU_PKG_LIBRARY_DIRS  ${GAMMU_ROOT}/libgammu/Debug)#TODO : gérer les configurations
   set(GAMMU_PKG_LIBRARIES Gammu.lib)
   set(GAMMU_ADDITIONAL_LIBRARIES  ${GAMMU_ROOT}/helper/Debug/string.lib ${GAMMU_ROOT}/helper/Debug/win32dirent.lib)#TODO : gérer les configurations
else(WIN32)
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
endif(WIN32)

find_path(GAMMU_INCLUDE_DIRS NAMES gammu.h
   PATHS
   ${GAMMU_PKG_INCLUDE_DIRS}
)

find_library(GAMMU_LIBRARIES NAMES ${GAMMU_PKG_LIBRARIES}
   PATHS
   ${GAMMU_PKG_LIBRARY_DIRS}
)

# Add additional libraries
set(GAMMU_LIBRARIES ${GAMMU_LIBRARIES} ${GAMMU_ADDITIONAL_LIBRARIES})

# handle the QUIETLY and REQUIRED arguments and set GAMMU_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gammu DEFAULT_MSG GAMMU_INCLUDE_DIRS GAMMU_LIBRARIES)

mark_as_advanced(GAMMU_INCLUDE_DIRS GAMMU_LIBRARIES)

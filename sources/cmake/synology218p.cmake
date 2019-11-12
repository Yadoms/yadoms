# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_CROSSCOMPILING True)
set(CMAKE_COMPILER_IS_RASPBERRY_CROSS_COMPILER ON)

# User specific configuration
include(CMakeListsUserConfig.txt OPTIONAL)

# cross compiler tools
set(CC_RPI_GCC ${CC_RPI_ROOT}/bin/x86_64-pc-linux-gnu-gcc)
set(CC_RPI_GXX ${CC_RPI_ROOT}/bin/x86_64-pc-linux-gnu-g++)
set(CC_RPI_LIBS ${CC_RPI_ROOT}/x86_64-pc-linux-gnu/sys-root)

# specify the cross compiler
SET(CMAKE_C_COMPILER   ${CC_RPI_GCC})
SET(CMAKE_CXX_COMPILER ${CC_RPI_GXX})

message(STATUS "Cross building for Synology 218+")
message(STATUS "CC_RPI_ROOT : ${CC_RPI_ROOT}")
message(STATUS "CC_RPI_GCC : ${CC_RPI_GCC}")
message(STATUS "CC_RPI_GXX : ${CC_RPI_GXX}")
message(STATUS "CC_RPI_LIBS : ${CC_RPI_LIBS}")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#define the systemname (for good package name)
set(CMAKE_PACKAGE_PLATFORM_NAME "Synology218p")

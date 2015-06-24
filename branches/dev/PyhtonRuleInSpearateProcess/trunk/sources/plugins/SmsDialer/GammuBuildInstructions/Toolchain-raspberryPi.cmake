# Toolchain configuration for Raspberry Pi

# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

# path to the toolchain ==> adapt it to your toolchain path
SET(PATH_TO_RASPBERRY_TOOLCHAIN /home/ubuntu/domotique/raspberrypi/tools/trunk)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER ${PATH_TO_RASPBERRY_TOOLCHAIN}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${PATH_TO_RASPBERRY_TOOLCHAIN}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH ${PATH_TO_RASPBERRY_TOOLCHAIN}/arm-bcm2708/arm-bcm2708-linux-gnueabi/arm-bcm2708-linux-gnueabi/sysroot/)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Disable pkg-config lookups
set(PKG_CONFIG_EXECUTABLE /bin/false)

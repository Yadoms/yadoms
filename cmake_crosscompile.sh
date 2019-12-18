#!/bin/bash

#move to project directory
BASEDIR=$(dirname $0);
set -e
cd $BASEDIR/projects

#treat script parameters
if [ -z "$1" -o -z "$2" -o -z "$3" ]
then
	echo "Usage"
	echo " cmake_crosscompile.h <platformName> <arch_prefix> <toolchainPath> [<buildType>]"
	echo " ------------- "
	echo " Example :"
	echo "    cmake_crosscompile RaspberryPI arm-linux-gnueabihf- /opt/raspberrypi.cmake "
	echo "    cmake_crosscompile RaspberryPI arm-linux-gnueabihf- /opt/raspberrypi.cmake Release"
	echo "    cmake_crosscompile Synology218p x86_64-pc-linux-gnu- /opt/synology218p.cmake Release"
	exit 1
fi

#define script variables
platformName=$1
arch_prefix=$2
toolchainPath=$3

if [ -z "$4" ]
then
	buildType=
else
	buildType=$4
fi

#manage cmake executable
getCMakeExecutable () {
   local cmakePath=$( grep -oP '^[\t ]*set\(ALTERNATIVE_CMAKE_ROOT[[:space:]]\"\K.*(?=")' ../sources/CMakeListsUserConfig.txt )
   if [ -z "$cmakePath" ]
   then
      echo cmake
   else
      echo $cmakePath/bin/cmake
   fi
}
cmake_executable=$(getCMakeExecutable)
echo "Use cmake executable $cmake_executable"
$cmake_executable --version

#run cmake
cmake -DCMAKE_BUILD_TYPE="$buildType" -DCMAKE_YADOMS_PLATFORM=$platformName -DCC_RPI_GCC=$arch_prefix-gcc -DCC_RPI_GXX=$arch_prefix-g++  -DCMAKE_TOOLCHAIN_FILE=.$toolchainPath ../sources


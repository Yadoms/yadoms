#!/bin/bash

#move to project directory
BASEDIR=$(dirname $0);
set -e
cd $BASEDIR/projects

#treat script parameter (if there is one, then directly use it for cases)
if test "$#" -ne 3; then
    echo "Illegal number of parameters"
	echo "Usage : \"sh cmake_cross.sh platformName pathToCmakeToolchain buildType\""
	echo "       platformName : The platform name : raspberryPI, Synology218p,...."
	echo "       pathToCmakeToolchain : The path to the toolchain file (needed for cross compiling)"
	echo "       buildType : The build type (Debug, Release,...) => values are cmake CMAKE_BUILD_TYPE allowed values"
	exit 1
fi

# Program args
#	$1 : platform name 					ex : RaspberryPI
#	$2 : toolchain path (absolute)		ex : source/raspberry.cmake
#   $3 : build type : [Debug; Release]  ex : Release



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

# cmake for cross compilation RELEASE
#cmake -DCMAKE_BUILD_TYPE="$3" -DCMAKE_YADOMS_PLATFORM=$1 -DCC_RPI_GCC=arm-linux-gnueabihf-gcc -DCC_RPI_GXX=arm-linux-gnueabihf-g++  -DCMAKE_TOOLCHAIN_FILE=$2 ../sources
cmake -DCMAKE_BUILD_TYPE="$3" -DCMAKE_YADOMS_PLATFORM=$1 -DCMAKE_TOOLCHAIN_FILE=$2 ../sources





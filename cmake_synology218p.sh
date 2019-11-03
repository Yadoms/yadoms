#!/bin/bash

#move to project directory
BASEDIR=$(dirname $0);
set -e
cd $BASEDIR/projects

#treat script parameter (if there is one, then directly use it for cases)
if [ -z "$1" ]
then
	echo "Which choice would you like?"
	echo " -> Generate a linux makefile for Cross Compilation (c)"
	echo " -> Generate a linux makefile for Cross Compilation RELEASE (r)"
	read choice
else
	choice=$1
fi

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

#run cmake depending on user choice (or script parameter)
case "$choice" in

    c)
	# cmake for cross compilation
	$cmake_executable  -DCMAKE_YADOMS_PLATFORM=Synology218p -DCC_RPI_GCC=x86_64-pc-linux-gnu-gcc -DCC_RPI_GXX=x86_64-pc-linux-gnu-g++  -DCMAKE_TOOLCHAIN_FILE=../sources/cmake/synology218p.cmake ../sources
	;;

    r)
	# cmake for cross compilation RELEASE
	cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_YADOMS_PLATFORM=Synology218p -DCC_RPI_GCC=x86_64-pc-linux-gnu-gcc -DCC_RPI_GXX=x86_64-pc-linux-gnu-g++  -DCMAKE_TOOLCHAIN_FILE=../sources/cmake/synology218p.cmake ../sources
	;;

    *)
	# generate an error and return 1
	echo "$choice is not a valid choice"
	exit 1
	;;
esac


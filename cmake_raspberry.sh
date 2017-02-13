#!/bin/bash

#move to project directory
BASEDIR=$(dirname $0);
set -e
cd $BASEDIR/projects

#treat script parameter (if there is one, then directly use it for cases)
if [ -z "$1" ]
then
	echo "Which choice would you like?"
	echo " -> Generate a linux makefile to run on RaspberryPI (p)"
	echo " -> Generate a linux makefile for Cross Compilation (c)"
	echo " -> Generate Eclipse CDT4 project files for CrossCompilation (e)"
	read choice
else
	choice=$1
fi

#run cmake depending on user choice (or script parameter)
case "$choice" in

    p)
	# cmake for makefile
	cmake -DCMAKE_YADOMS_PLATFORM=Raspberry -DCMAKE_CXX_COMPILER=g++-4.9 -DCMAKE_CC_COMPILER=gcc-4.9 ../sources
	;;

    c)
	# cmake for cross compilation
	cmake  -DCMAKE_YADOMS_PLATFORM=Raspberry -DCC_RPI_GCC=arm-linux-gnueabihf-gcc -DCC_RPI_GXX=arm-linux-gnueabihf-g++  -DCMAKE_TOOLCHAIN_FILE=../sources/raspberrypi.cmake ../sources
	;;

    e)
	# cmake for compilation and debug with Eclipse
	cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_YADOMS_PLATFORM=Raspberry -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../sources
	;;
	
    *)
	# generate an error and return 1
	echo "$choice is not a valid choice"
	exit 1
	;;
esac


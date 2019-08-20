#!/bin/bash

#move to project directory
BASEDIR=$(dirname $0);
set -e
cd $BASEDIR/projects

#treat script parameter (if there is one, then directly use it for cases)
if [ -z "$1" ]
then
	echo "Which choice would you like?"
	echo " -> Generate a makefile (m)"
	echo " -> Generate a makefile RELEASE (r)"
	echo " -> Generate a makefile for crosscompilation (osxcroos) (c)"
	echo " -> Generate a makefile for crosscompilation (osxcroos) RELEASE (d)"
	echo " -> Generate a xcode project (x)"
	read choice
else
	choice=$1
fi

#run cmake depending on user choice (or script parameter)
case "$choice" in

    m)
	# cmake for makefile
	cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ../sources
	;;

    r)
	# cmake for makefile
	cmake -DCMAKE_BUILD_TYPE="Release" -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ../sources
	;;
	
    c)
	#run osxcross-conf to get all env var and set OSXCROSS_HOST (needed by osxcross)
	eval "`x86_64-apple-darwin16-osxcross-conf`"
	export OSXCROSS_HOST="x86_64-apple-darwin16"
	# cmake for makefile (cross compilation)
	cmake -D CMAKE_C_COMPILER=x86_64-apple-darwin16-gcc -D CMAKE_CXX_COMPILER=x86_64-apple-darwin16-g++ -DCMAKE_TOOLCHAIN_FILE=../sources/cmake/ccmacos.cmake ../sources
	;;	

    d)
	#run osxcross-conf to get all env var and set OSXCROSS_HOST (needed by osxcross)
	eval "`x86_64-apple-darwin16-osxcross-conf`"
	export OSXCROSS_HOST="x86_64-apple-darwin16"
	# cmake for makefile (cross compilation)
	cmake -DCMAKE_BUILD_TYPE="Release" -D CMAKE_C_COMPILER=x86_64-apple-darwin16-gcc -D CMAKE_CXX_COMPILER=x86_64-apple-darwin16-g++ -DCMAKE_TOOLCHAIN_FILE=../sources/cmake/ccmacos.cmake ../sources
	;;	

    x)
	# cmake for xcode
	cmake -GXcode ../sources
	;;
    *)
	# generate an error and return 1
	echo "$choice is not a valid choice"
	exit 1
	;;
esac


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


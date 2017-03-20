#!/bin/bash

#move to project directory
BASEDIR=$(dirname $0);
set -e
cd $BASEDIR/projects

#treat script parameter (if there is one, then directly use it for cases)
if [ -z "$1" ]
then
	echo "Which choice would you like?"
	echo " -> Generate a linux makefile (m)"
	echo " -> Generate a linux makefile RELEASE (r)"
	echo " -> Generate a Codeblocks project (c)"
	echo " -> Generate Eclpise CDT4 project files (e)"
	read choice
else
	choice=$1
fi

#run cmake depending on user choice (or script parameter)
case "$choice" in

    m)
	# cmake for makefile
	cmake ../sources
	;;

    r)
	# cmake for makefile
	cmake -DCMAKE_BUILD_TYPE="Release" ../sources
	;;

    c)
	# cmake for codeblocks
	cmake  -G "CodeBlocks - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../sources
	;;
	
    e)
	# cmake for compilation and debug with Eclipse
	cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug  -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../sources
	;;
	
    *)
	# generate an error and return 1
	echo "$choice is not a valid choice"
	exit 1
	;;
esac


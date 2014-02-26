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
	read choice
else
	choice=$1
fi

#run cmake depending on user choice (or script parameter)
case "$choice" in

    p)
	# cmake for makefile
	cmake -DCMAKE_CXX_COMPILER=g++-4.7 -DCMAKE_CC_COMPILER=gcc-4.7 ../sources
	;;
    c)
	# cmake for cross compilation
	cmake  ../sources
	;;
    *)
	# generate an error and return 1
	echo "$choice is not a valid choice"
	exit 1
	;;
esac


#!/bin/bash


BASEDIR=$(dirname $0);
set -e

cd $BASEDIR/projects
cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ../sources




#!/bin/bash


BASEDIR=$(dirname $0);
set -e

cd $BASEDIR/projects

cmake -DCMAKE_CXX_COMPILER=g++-4.7 -DCMAKE_CC_COMPILER=gcc-4.7 ../sources




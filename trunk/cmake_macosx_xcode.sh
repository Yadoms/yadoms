#!/bin/bash


BASEDIR=$(dirname $0);
set -e

cd $BASEDIR/projects
cmake -GXcode ../sources




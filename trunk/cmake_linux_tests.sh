#!/bin/bash


BASEDIR=$(dirname $0);
set -e

cd $BASEDIR/tests
cmake ../tests

#!/bin/bash
set -e

cd /work/tests/unit

echo "Copy build config file"
cp $YADOMS_DEPS_PATH/CMakeListsUserConfig.txt sources/
cp /ccmacos.cmake sources

echo "Display config content"
cat sources/CMakeListsUserConfig.txt

echo "Create makefile"
sh cmake_macosx_tests.sh d

echo "Building Yadoms unit tests"
cd projects
OSXCROSS_MP_INC=1 make all_unity
cd -

chmod 777 -R /work/tests/unit/builds

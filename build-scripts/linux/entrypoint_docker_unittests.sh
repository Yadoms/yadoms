#!/bin/bash
set -e

cd /work/tests/unit

echo "Copy build config file"
cp $YADOMS_DEPS_PATH/CMakeListsUserConfig.txt sources/

echo "Display config content"
cat sources/CMakeListsUserConfig.txt

echo "Create makefile"
sh cmake_linux_tests.sh m

echo "Building Yadoms unit tests"
cd projects
make all
cd -

chmod 777 -R /work/tests/unit/builds

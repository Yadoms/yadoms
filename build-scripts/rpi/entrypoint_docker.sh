#!/bin/bash
set -e

cd /work

echo "Copy build config file"
cp $YADOMS_DEPS_PATH/CMakeListsUserConfig.txt sources/
#cp /raspberrypi.cmake sources/cmake

echo "Create makefile"
sh cmake_raspberry.sh r

echo "Build Yadoms"
cd projects
make all_unity
cd -

if [ $MAKE_PACKAGE == "true" ]; then
	echo "Build Yadoms package"
	cd projects
	make package
	cd -
	
	cd update
	sh make_package.sh RaspberryPI
	cd -

	echo "Prepare to generate raspberryPI SD image"
	
	#fix binfmt (cannot be done in Dockerfile, so do it in entrypoint)
	mount -v binfmt_misc -t binfmt_misc /proc/sys/fs/binfmt_misc/
	dpkg-reconfigure -u binfmt-support qemu qemu-user-static
	update-binfmts --enable qemu-arm

	#clone the repo
	if [ -z "$PARAM1" ]
	then
		echo "Cloning Yadoms/yadoms-build_raspberrypi_image.git on default branch"
		git -c http.sslVerify=false clone --depth=1 https://github.com/Yadoms/yadoms-build_raspberrypi_image.git
	else
		echo "Cloning Yadoms/yadoms-build_raspberrypi_image.git on branch : $PARAM1"
		git -c http.sslVerify=false clone --depth=1 -b $PARAM1 https://github.com/Yadoms/yadoms-build_raspberrypi_image.git
	fi
	cd yadoms-build_raspberrypi_image

	#make the image
	./create-yadoms-pi-image /work

fi

#!/bin/sh

BASEDIR=$(dirname $0);
set -e

if [ -z "$1" ]
then
	echo "Make package require the current plaform name"
	echo " make_package.sh <platform>"
	echo " example"
	echo "         : make_package.sh Linux"
	echo "         : make_package.sh RaspberryPI"
	exit 1
else
	#move to project root directory
	cd $BASEDIR/..
	echo "Build Yadoms update package"
	mkdir updatepackage
	yadomsVersion=$(grep -oP '###[[:space:]]\K.*' sources/server/changelog.md -m 1)
	# Copy script
	cp update/scripts/update.sh updatepackage/update.sh
	# Generate package.json
	cp update/package.json.in updatepackage/package.json
	sed -i -- 's/__version__/'$yadomsVersion'/g' updatepackage/package.json
	sed -i -- 's/__gitdate__/'`git log -1 --format=%cI `'/g' updatepackage/package.json
	cp sources/server/changelog.md updatepackage/changelog.md
	mv builds/package packagetomove
	mv builds updatepackage/package
	rm -f updatepackage/package/yadoms.ini
	cd updatepackage
	zip -qr ../package-$1.zip ./ -x \*.gitignore
	cd -
	mkdir builds
	mv packagetomove builds/package
	mv package-$1.zip builds/package
	#cleanup
	rm -Rf updatepackage
fi

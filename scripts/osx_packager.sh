#!/bin/bash

if [ -z ${1} ]
then
	echo "No version number specified!!!\n"
	exit 1
fi


TARGET_DIR=package

rm -rf ${TARGET_DIR}
mkdir -p ${TARGET_DIR}
mkdir -p releases

cp -R deploy/KISS.app ${TARGET_DIR}

hdiutil mount "scripts/KISS IDE Install Image.sparseimage"
cp -R deploy/KISS.app/Contents /Volumes/KISS\ IDE/KISS.app/
# hdiutil unmount /Volumes/KISS\ IDE
hdiutil detach /Volumes/KISS\ IDE
sleep 2s
hdiutil convert "scripts/KISS IDE Install Image.sparseimage" -format UDBZ -o "releases/KISS IDE ${1}.dmg"

rm -Rf package

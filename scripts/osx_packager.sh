#!/bin/bash

if [ -z ${1} ]
then
	echo "No version number specified!!!\n"
	exit 1
fi


TARGET_DIR=package/KISS-${1}

rm -rf package
mkdir -p ${TARGET_DIR}
mkdir -p releases

cp -R deploy/* ${TARGET_DIR}

hdiutil create -srcfolder package -volname "KISS ${1}" releases/KISS-${1}.dmg


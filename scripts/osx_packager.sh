#!/bin/bash

if [ -z ${1} ]
then
	echo "No version number specified!!!\n"
	exit 1
fi


TARGET_DIR=package
SPARSEIMAGE=scripts/kiss.sparseimage

echo "Using sparse image ${SPARSEIMAGE}" 

rm -rf ${TARGET_DIR}
mkdir -p ${TARGET_DIR}
mkdir -p releases

cp -R deploy/KISS.app ${TARGET_DIR}
hdiutil resize -size 300m $SPARSEIMAGE
hdiutil mount $SPARSEIMAGE
cp -R deploy/KISS.app/Contents /Volumes/KISS\ IDE/KISS.app/
# hdiutil unmount $MOUNT
hdiutil detach /Volumes/KISS\ IDE
sleep 2s
hdiutil convert $SPARSEIMAGE -format UDBZ -o "releases/KISS IDE ${1}.dmg"
sleep 2s
hdiutil mount $SPARSEIMAGE
rm -Rf /Volumes/KISS\ IDE/KISS.app/Contents
hdiutil detach /Volumes/KISS\ IDE
hdiutil compact $SPARSEIMAGE
rm -Rf package

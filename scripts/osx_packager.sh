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

title="KISS ${1}"

hdiutil create -srcfolder package -volname "${title}" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${2}m pack.temp.dmg
device=$(hdiutil attach -readwrite -noverify -noautoopen "pack.temp.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')

echo '
   tell application "Finder"
     tell disk "'${title}'"
           open
           set current view of container window to icon view
           set toolbar visible of container window to false
           set statusbar visible of container window to false
           set the bounds of container window to {400, 100, 885, 430}
           set theViewOptions to the icon view options of container window
           set arrangement of theViewOptions to not arranged
           set icon size of theViewOptions to 72
           set background picture of theViewOptions to file ".background:scripts/dmg-background.png"
           make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
           set position of item "KISS" of container window to {100, 100}
           set position of item "Applications" of container window to {375, 100}
           update without registering applications
           delay 5
           eject
     end tell
   end tell
' | osascript

rm -Rf package

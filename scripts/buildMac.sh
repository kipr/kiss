#!/bin/sh

if [ $# -ne 1 ] 
then
	echo "Usage: ${0} [version]"
	exit 0
fi

echo "-- Configuring for library build -- "
qmake -recursive -spec macx-g++ CONFIG+=library VERSION=${1}
echo "-- Making library build --"
make
make install
echo "-- Configuring for application build --"
qmake -recursive -spec macx-g++ CONFIG+=application
echo "-- Making application build --"
make
make install
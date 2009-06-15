#!/bin/bash

if [ -z $1 ]
then
	echo "No argument!!"
	exit 0 
fi

install_framework_paths()
{
        if [ -z $1 ]
        then
                echo "install_framework_paths: No arg specified"
                return 1
        fi

        TARGET=$1
        shift
        FRAMEWORK_LIST=$@

        for i in ${FRAMEWORK_LIST}
        do
                install_name_tool -change ${i}.framework/Versions/4/${i} @executable_path/../Frameworks/${i} ${TARGET}
        done

}

copy_frameworks()
{
	if [ -z $1 ]
	then
		echo "No target specified"
		return 1
	fi

	if [ -z $2 ]
	then
		echo "No frameworks specified"
	fi

	TARGET_DIR=$1
	shift
	FRAMEWORKS_LIST=$@
	FRAMEWORKS_DIR=${TARGET_DIR}/Contents/Frameworks

	mkdir -p ${FRAMEWORKS_DIR}

	for i in ${FRAMEWORKS_LIST}
	do
		cp /Library/Frameworks/${i}.framework/Versions/4/${i} ${FRAMEWORKS_DIR}/${i}
		install_name_tool -id @executable_path/../Frameworks/${i} ${FRAMEWORKS_DIR}/${i}
		install_framework_paths ${FRAMEWORKS_DIR}/${i} ${FRAMEWORKS_LIST}
	done
}

APP_DIR=$1
shift
FRAMEWORKS=$@

copy_frameworks ${APP_DIR} ${FRAMEWORKS}
install_framework_paths ${APP_DIR}/Contents/MacOS/KISS ${FRAMEWORKS}

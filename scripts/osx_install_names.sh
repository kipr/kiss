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
                install_name_tool -change /Library/Frameworks/${i} @executable_path/../Frameworks/${i} ${TARGET}
		install_name_tool -change ${i} @executable_path/../Frameworks/${i} ${TARGET}
        done

}

TARGET=$1
shift
FRAMEWORKS=$@

install_framework_paths ${TARGET} ${FRAMEWORKS}

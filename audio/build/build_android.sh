#!/bin/sh
set -e
LOCAL_PATH=$(pwd)
BIN_PATH=$(pwd)/../bin
export NDK_PROJECT_PATH=$(pwd)/app


echo "---------------------------------------------"
export APPLICATION_NAME=server

echo ${APPLICATION_NAME}
NDK_PATH=~/android-ndk-r8e/

cd ${NDK_PATH} 
./ndk-build

cp ${NDK_PROJECT_PATH}/obj/local/armeabi/* ${BIN_PATH}
echo "---------------------------------------------"



echo "Done"



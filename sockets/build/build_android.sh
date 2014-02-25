#!/bin/sh
set -e
LOCAL_PATH=$(pwd)
BIN_PATH=$(pwd)/../bin
export NDK_PROJECT_PATH=$(pwd)/app


echo "---------------------------------------------"
export APPLICATION_NAME=server

echo ${APPLICATION_NAME}
NDK_PATH=~/work/android-ndk-r9d/

cd ${NDK_PATH} 
./ndk-build
mkdir -p ${BIN_PATH}
cp ${NDK_PROJECT_PATH}/obj/local/armeabi/* ${BIN_PATH}
echo "---------------------------------------------"



echo "Done"



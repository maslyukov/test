#!/bin/sh

./build_android.sh
adb push ../bin/stream_server /data/local/tmp/stream
adb shell /data/local/tmp/stream/stream_server restart
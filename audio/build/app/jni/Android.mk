#LOCAL_PATH is used to locate source files in the development tree.
#the macro my-dir provided by the build system, indicates the path of the current directory

LOCAL_PATH:=$(call my-dir)

################################################
#          build application code              #
################################################

include $(CLEAR_VARS)

LOCAL_MODULE := $(APPLICATION_NAME)
LOCAL_LDLIBS :=-llog -lm -landroid -lOpenSLES
LOCAL_SHARED_LIBRARIES := $(LIBMNL) $(LIBSQLITE3)
LOCAL_CPPFLAGS := -std=c++11  -fexceptions -frtti -lpthread
#-D_STLP_USE_BOOST_SUPPORT
#-D__GXX_EXPERIMENTAL_CXX0X__=1

#/sources/cxx-stl/gnu-libstdc++/4.7/include
#		$(NDK_PATH)/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi/include \ 
#		$(NDK_PATH)/sources/cxx-stl/gnu-libstdc++/4.7/include \

LOCAL_C_INCLUDES := \
		$(NDK_PROJECT_PATH)/../../include/ \

LOCAL_SRC_FILES:=\
		../../../src/audio/Audio.cpp\
		../../../src/audio/WAVParser.cpp\
		../../../src/main.cpp \

#		../../../src/filesystem_observer/IWatchFactory.cpp\

include $(BUILD_EXECUTABLE)








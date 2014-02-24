LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := Asteroids
LOCAL_SRC_FILES := engine.cpp engineAndroid.cpp aux.cpp render.cpp game.cpp
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_CFLAGS := -DPLATFORM_ANDROID
include $(BUILD_SHARED_LIBRARY)
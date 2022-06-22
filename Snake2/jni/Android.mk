LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Snake
LOCAL_SRC_FILES := Snake.cpp

include $(BUILD_SHARED_LIBRARY)

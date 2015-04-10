LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := mp3util
LOCAL_SRC_FILES := types.c utils.c header.c frame.c id3v2lib.c mp3util.c
LOCAL_LDLIBS    := -lm -llog

include $(BUILD_SHARED_LIBRARY)
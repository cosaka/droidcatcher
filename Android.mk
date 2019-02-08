LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := droidcatcher
LOCAL_SRC_FILES := src/main.c
LOCAL_CPPFLAGS :=
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
include $(BUILD_EXECUTABLE)

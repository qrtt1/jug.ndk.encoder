LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := audio_encoder
LOCAL_SRC_FILES := audio_encoder.c

LOCAL_LDLIBS := -lm -llog
LOCAL_LDLIBS += $(NDK_APP_PROJECT_PATH)/jni/libmp3lame.so

LOCAL_C_INCLUDES += $(NDK_APP_PROJECT_PATH)/jni/lame

include $(BUILD_SHARED_LIBRARY)



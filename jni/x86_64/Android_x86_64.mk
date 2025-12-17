LOCAL_PATH := $(call my-dir)

### ソースコードをビルドする ###
include $(CLEAR_VARS)
LOCAL_MODULE            := mocopi_motion_serializer_shared
LOCAL_MODULE_FILENAME   := libmocopi_motion_serializer
LOCAL_CFLAGS            := -Werror
# ソースコード読み込み
# CPP_FILES               := $(shell find $(LOCAL_PATH)/../../main -name *.cpp)
CPP_FILES               := \
  $(wildcard $(LOCAL_PATH)/../../main/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../main/**/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../main/**/**/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../main/**/**/**/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../main/**/**/**/**/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../main/**/**/**/**/**/*.cpp)
LOCAL_SRC_FILES         := $(CPP_FILES:$(LOCAL_PATH)/%=%)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../main

LOCAL_LDLIBS    		:= -llog
LOCAL_LDFLAGS += "-Wl,-z,max-page-size=16384"
include $(BUILD_SHARED_LIBRARY)
### ソースコードをビルドする（ここまで） ###

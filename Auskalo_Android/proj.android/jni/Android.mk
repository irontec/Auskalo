LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/Helpers/*.cpp)
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/Helpers/*.c)
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/Models/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.c)

LOCAL_SRC_FILES := hellocpp/main.cpp

LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes/Helpers \
		    $(LOCAL_PATH)/../../Classes/Models \
		    $(LOCAL_PATH)/../../Classes \
		    /Users/imendilibar/Documents/Cocos/cocos2d-2.0-x-2.0.4/cocos2dx/cocoa \
		   /Users/imendilibar/Documents/Cocos/cocos2d-2.0-x-2.0.4/cocos2dx/touch_dispatcher

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static cocos_curl_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx)
$(call import-module,extensions)

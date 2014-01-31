LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_CFLAGS += -DCOCOS2D_DEBUG=1

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/Helpers/SQLite/sqlite3.c \
../../Classes/Helpers/json/cJSON.c \
../../Classes/Helpers/CCScrollLayer.cpp \
../../Classes/Models/MultzoaModel.cpp \
../../Classes/Helpers/Extensions/SlidingMenu.cpp \
../../Classes/Helpers/SQLHelper.cpp \
../../Classes/Helpers/SpriteButton.cpp \
../../Classes/Helpers/TrivialAPI.cpp \
../../Classes/Helpers/VisibleRect.cpp \
../../Classes/Layers/BukatutaLayer.cpp \
../../Classes/Layers/MainMenuLayer.cpp \
../../Classes/Models/GalderaModel.cpp \
../../Classes/Models/KategoriaModel.cpp \
../../Classes/Scenes/BukatutaScene.cpp \
../../Classes/Scenes/CategoryScene.cpp \
../../Classes/Scenes/GalderaScene.cpp \
../../Classes/Scenes/LevelScene.cpp \
../../Classes/Scenes/LoginScene.cpp \
../../Classes/Scenes/MainLoadingScene.cpp \
../../Classes/Scenes/MainMenuScene.cpp \
../../Classes/Scenes/ProfilaScene.cpp \
/Applications/cocos2d-x-2.1.5/cocos2dx/platform/android/jni/Urljni.cpp \

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Classes//Helpers \
                        /Applications/cocos2d-x-2.1.5/cocos2dx/cocoa \
/Applications/cocos2d-x-2.1.5/cocos2dx/touch_dispatcher

LOCAL_STATIC_LIBRARIES += curl_static_prebuilt

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static cocos_curl_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)


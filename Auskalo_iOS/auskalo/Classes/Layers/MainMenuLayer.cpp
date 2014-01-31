//
//  MainMenuLayer.cpp
//  TrivialJokoa
//
//  Created by iker on 11/12/12.
//
//

#include "MainMenuLayer.h"
#include "../Scenes/CategoryScene.h"
#include "../Scenes/ProfilaScene.h"
#include "CCApplication.h"
#include "SimpleAudioEngine.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "/Applications/cocos2d-x-2.1.5/cocos2dx/platform/android/jni/Urljni.h"
#endif


using namespace CocosDenshion;

MainMenuLayer::MainMenuLayer()
{
    
}

MainMenuLayer::~MainMenuLayer()
{
    
}


bool MainMenuLayer::init()
{
    return true;
}

void MainMenuLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{

}

SEL_CallFuncN MainMenuLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

SEL_MenuHandler MainMenuLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MainMenuLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "playClicked", MainMenuLayer::playClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "profilaClicked", MainMenuLayer::profilaClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buruzClicked", MainMenuLayer::buruzClicked);
    return NULL;
}

void MainMenuLayer::playClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    CategoryScene *scene = CategoryScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void MainMenuLayer::profilaClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    ProfilaScene *scene = ProfilaScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void MainMenuLayer::buruzClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        openURLJNI("http://auskalo.net/honi-buruz/");
    #else
        CCApplication::sharedApplication()->openURL("http://auskalo.net/honi-buruz/");
    #endif
    
}

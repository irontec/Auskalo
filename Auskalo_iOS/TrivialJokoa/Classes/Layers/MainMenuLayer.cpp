//
//  MainMenuLayer.cpp
//  TrivialJokoa
//
//  Created by iker on 11/12/12.
//
//

#include "MainMenuLayer.h"
#include "CategoryScene.h"
#include "ProfilaScene.h"
#include "CCApplication.h"
#include "SimpleAudioEngine.h"

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


SEL_MenuHandler MainMenuLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MainMenuLayer::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "playClicked", MainMenuLayer::playClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "profilaClicked", MainMenuLayer::profilaClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buruzClicked", MainMenuLayer::buruzClicked);
    return NULL;
}

void MainMenuLayer::playClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    CategoryScene *scene = CategoryScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void MainMenuLayer::profilaClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    ProfilaScene *scene = ProfilaScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void MainMenuLayer::buruzClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    CCApplication::openURL("http://auskalo.net/honi-buruz/");
}

//
//  CocosBuilderExampleAppDelegate.cpp
//  CocosBuilderExample
//
//  Created by iker on 15/11/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "MainLoadingScene.h"

#include "MainMenuScene.h"
#include "GalderaScene.h"

USING_NS_CC;

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(320, 480),   "iphone" };
//static Resource androidMediumResource  =  { cocos2d::CCSizeMake(480, 800),   "medium" };
static Resource mediumResource =  { cocos2d::CCSizeMake(768, 1024),  "ipad"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(1536, 2048), "ipadhd" };

//static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(320, 480);
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(320, 480);

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
    
    CCSize frameSize = pEGLView->getFrameSize();
    
    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.
    
    // if the frame's height is larger than the height of medium resource size, select large resource.
    if (frameSize.height > mediumResource.size.height)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(largeResource.size.height/designResolutionSize.height);
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
        pDirector->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);
        pDirector->setContentScaleFactor(smallResource.size.height/designResolutionSize.height);
    }
   
    MainLoadingScene *scene = MainLoadingScene::create();
    
    //LevelScene *scene = LevelScene::create();
    
    pDirector->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

//
//  CocosBuilderExampleAppDelegate.cpp
//  CocosBuilderExample
//
//  Created by iker on 15/11/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "Scenes/MainLoadingScene.h"
#include "AppMacros.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
    
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
    
    CCSize frameSize = pEGLView->getFrameSize();
    
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    
    pDirector->setAnimationInterval(1.0 / 60);
    
    std::vector<std::string> searchPath;
    
    if (frameSize.height > mediumResource.size.height) {
        searchPath.push_back(largeResource.directory);
        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	} else if (frameSize.width > smallResource.size.width) {
        searchPath.push_back(mediumResource.directory);
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    } else {
        searchPath.push_back(smallResource.directory);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(searchPath);
    
    CCScene *scene = MainLoadingScene::create();
    
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

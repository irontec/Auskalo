//
//  MainLoadingScene.cpp
//  TrivialJokoa
//
//  Created by iker on 11/12/12.
//
//

#include "MainLoadingScene.h"
#include "MainMenuScene.h"
#include "LoginScene.h"
#include "MainMenuScene.h"
#include "CCNodeLoaderLibrary.h"

USING_NS_CC;
USING_NS_CC_EXT;

MainLoadingScene::~MainLoadingScene()
{
    if (_api) {
        _api->release();
        _api = NULL;
    }
}

MainLoadingScene::MainLoadingScene()
{
    _api = new TrivialAPI();
    CCCallFunc *pCallback = CCCallFunc::create(this, callfunc_selector(MainLoadingScene::finishLoading));
    _api->setCallback(pCallback);
    _api->updateData();
}


// on "init" you need to initialize your instance
bool MainLoadingScene::init()
{
    CCNodeLoaderLibrary *pNodeLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    pNodeLibrary->registerCCNodeLoader("MainLoadingLayer", MainLoadingLayerLoader::loader());
    
    CCBReader *pReader = new CCBReader(pNodeLibrary);
    pReader->autorelease();

    CCNode *node = pReader->readNodeGraphFromFile("ccbi/MainLoading.ccbi");
    
    this->addChild(node);
    
	return true;
}

void MainLoadingScene::finishLoading()
{
    CCScene *scene;
    
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    std::string username = pUserDefaults->getStringForKey("username");
    
    if (username.length() == 0) {
        scene = LoginScene::create();
    } else {
        scene = MainMenuScene::create();
    }
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

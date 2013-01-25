//
//  MainMenuScene.cpp
//  TrivialJokoa
//
//  Created by iker on 12/12/12.
//
//

#include "MainMenuScene.h"
#include "MainMenuLayer.h"
#include "CCNodeLoaderLibrary.h"

MainMenuScene::~MainMenuScene()
{
   
}

MainMenuScene::MainMenuScene()
{
   
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    CCNodeLoaderLibrary *pNodeLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    pNodeLibrary->registerCCNodeLoader("MainMenuLayer", MainMenuLayerLoader::loader());
    
    pReader = new CCBReader(pNodeLibrary);
    
    MainMenuLayer *node = (MainMenuLayer*) pReader->readNodeGraphFromFile("ccbi/MainMenu.ccbi");
    
    CCBAnimationManager *animationManager =  pReader->getAnimationManager();
    node->animationManager = animationManager;

    this->addChild(node);

	return true;
}

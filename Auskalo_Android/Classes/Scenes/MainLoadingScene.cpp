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
#include "../Helpers/VisibleRect.h"

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
    CCCallFunc *pProgressCallback = CCCallFunc::create(this, callfunc_selector(MainLoadingScene::updateProgress));
    _api->setCallback(pCallback);
    _api->setProgressCallback(pProgressCallback);
    _api->updateData();
}


// on "init" you need to initialize your instance
bool MainLoadingScene::init()
{
    if ( !CCScene::init() )
    {
        return false;
    }
    
    CCLayerColor *layer = CCLayerColor::create();
    layer->initWithColor(ccc4(255, 255, 255, 255), VisibleRect::right().x , VisibleRect::top().y);
    layer->setPosition(VisibleRect::center());
    layer->setAnchorPoint(ccp(0.5f, 0.5f));
    layer->ignoreAnchorPointForPosition(false);
    this->addChild(layer, 0);
    
    CCSprite *fondo = CCSprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    this->addChild(fondo);

    
    CCSprite *sprite = CCSprite::create("logo.png");
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite);
    
    CCLabelTTF *progress = CCLabelTTF::create ("Kargatzen ari da.\nMesedez, minutu pare bat itxaron.", "fonts/PT_Sans-Web-Regular.ttf", 10);
    progress->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y + 12));
    progress->setAnchorPoint(ccp(0.5,0.5));
    progress->setColor(ccc3(0,0,0));
    addChild(progress);
    
    loading = CCSprite::create("loading.png");
    loading->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y + progress->getContentSize().height + loading->getContentSize().height / 2));
    addChild(loading);
    
    CCActionInterval* rotate = CCRotateBy::create(5.0f, 360.0f);
    CCRepeatForever *repeat = CCRepeatForever::create(rotate);
    loading->runAction(repeat);
    
	return true;
}

void MainLoadingScene::updateProgress()
{
//    std::ostringstream ss;
//    ss << _api->getProgress();
//    CCLog("Progress: %d", _api->getProgress());
//    std::string s(ss.str());
//    progress->setString(s.c_str());
}

void MainLoadingScene::finishLoading()
{
    CCScene *scene = MainMenuScene::create();

    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

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
    auto pCallback = CallFunc::create(CC_CALLBACK_0(MainLoadingScene::finishLoading, this));
    auto pProgressCallback = CallFunc::create(CC_CALLBACK_0(MainLoadingScene::updateProgress, this));
    _api->setCallback(pCallback);
    _api->setProgressCallback(pProgressCallback);
    _api->updateData();
}


// on "init" you need to initialize your instance
bool MainLoadingScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    LayerColor *layer = LayerColor::create();
    layer->initWithColor(Color4B(255, 255, 255, 255), VisibleRect::right().x , VisibleRect::top().y);
    layer->setPosition(VisibleRect::center());
    layer->setAnchorPoint(Vec2(0.5f, 0.5f));
    layer->setIgnoreAnchorPointForPosition(false);
    this->addChild(layer, 0);
    
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    this->addChild(fondo);

    
    Sprite *sprite = Sprite::create("logo.png");
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite);
    
    Label *progress = Label::createWithTTF ("Kargatzen ari da.\nMesedez, minutu pare bat itxaron.", "fonts/PT_Sans-Web-Regular.ttf", 10);
    progress->setPosition(Vec2(VisibleRect::center().x, VisibleRect::bottom().y + 12));
    progress->setAnchorPoint(Vec2(0.5,0.5));
    progress->setColor(Color3B(0,0,0));
    addChild(progress);
    
    loading = Sprite::create("loading.png");
    loading->setPosition(Vec2(VisibleRect::center().x, VisibleRect::bottom().y + progress->getContentSize().height + loading->getContentSize().height / 2));
    addChild(loading);
    
    ActionInterval* rotate = RotateBy::create(5.0f, 360.0f);
    RepeatForever *repeat = RepeatForever::create(rotate);
    loading->runAction(repeat);
    
	return true;
}

void MainLoadingScene::updateProgress()
{
//    std::ostringstream ss;
//    ss << _api->getProgress();
//    log("Progress: %d", _api->getProgress());
//    std::string s(ss.str());
//    progress->setString(s.c_str());
}

void MainLoadingScene::finishLoading()
{
    Scene *scene = MainMenuScene::create();

    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

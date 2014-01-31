//
//  MainMenuScene.cpp
//  TrivialJokoa
//
//  Created by iker on 12/12/12.
//
//

#include "MainMenuScene.h"
#include "../Layers/MainMenuLayer.h"
#include "CCNodeLoaderLibrary.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/SpriteButton.h"
#include "../Scenes/CategoryScene.h"
#include "../Scenes/ProfilaScene.h"

MainMenuScene::~MainMenuScene()
{
    
}

MainMenuScene::MainMenuScene()
{
    
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    CCSprite *fondo = CCSprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    addChild(fondo);
    
    CCSprite *leftGrass = CCSprite::create("grass_left.png");
    leftGrass->setAnchorPoint(ccp(0,0));
    leftGrass->setPosition(VisibleRect::leftBottom());
    addChild(leftGrass);
    CCSprite *rightGrass = CCSprite::create("grass_right.png");
    rightGrass->setAnchorPoint(ccp(1,0));
    rightGrass->setPosition(VisibleRect::rightBottom());
    addChild(rightGrass);
    
    CCSprite *spriteLogo = CCSprite::create("logo_opciones.png");
    spriteLogo->setPosition(ccp(VisibleRect::center().x, (VisibleRect::top().y - (spriteLogo->getContentSize().height / 2) - 20)));
    addChild(spriteLogo);
    
    int mySize = VisibleRect::center().y / 3;
    
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("botoia_normal.png");
    
    CCPoint anchorPoint = ccp(0.5f, 0.5f);
    
    CCLabelTTF *pLabel1 = CCLabelTTF::create("Jokatu", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    CCLabelTTF *pLabel2 = CCLabelTTF::create("Sailkapena", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    CCLabelTTF *pLabel3 = CCLabelTTF::create("Honi Buruz", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    
    // Button JOKATU
    SpriteButton *pButton1 = SpriteButton::create(texture ,this, callfuncO_selector(MainMenuScene::menuSelector), 1.0f);
    pButton1->setTag(1);
    pButton1->setAnchorPoint(anchorPoint);
    pButton1->setPosition(ccp(VisibleRect::center().x, (mySize * 3) - texture->getContentSize().height /2));
    pLabel1->setPosition(ccp(VisibleRect::center().x, (mySize * 3) - texture->getContentSize().height /2));
    
    // Button SAILKAPENA
    SpriteButton *pButton2 = SpriteButton::create(texture ,this, callfuncO_selector(MainMenuScene::menuSelector), 1.0f);
    pButton2->setTag(2);
    pButton2->setAnchorPoint(anchorPoint);
    pButton2->setPosition(ccp(VisibleRect::center().x, (mySize * 2) - texture->getContentSize().height /2));
    pLabel2->setPosition(ccp(VisibleRect::center().x, (mySize * 2) - texture->getContentSize().height /2));
    
    // Button HONI BURUZ
    SpriteButton *pButton3 = SpriteButton::create(texture ,this, callfuncO_selector(MainMenuScene::menuSelector), 1.0f);
    pButton3->setTag(3);
    pButton3->setAnchorPoint(anchorPoint);
    pButton3->setPosition(ccp(VisibleRect::center().x, mySize - texture->getContentSize().height /2));
    pLabel3->setPosition(ccp(VisibleRect::center().x, mySize - texture->getContentSize().height /2));
    
    addChild(pButton1);
    addChild(pLabel1);
    addChild(pButton2);
    addChild(pLabel2);
    addChild(pButton3);
    addChild(pLabel3);
    
	return true;
}

void MainMenuScene::menuSelector(CCObject* pSender)
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene;
    SpriteButton *selectedOpt = (SpriteButton*) pSender;
    CCScene *scene;
    switch (selectedOpt->getTag()) {
        case 1:
            scene = CategoryScene::create();
            tscene = CCTransitionFade::create(0.5, scene);
            pDirector->replaceScene(tscene);
            break;
        case 2:
            scene = ProfilaScene::create();
            tscene = CCTransitionFade::create(0.5, scene);
            pDirector->replaceScene(tscene);
            break;
        case 3:
            #ifdef __APPLE__
                CCApplication::sharedApplication()->openURL("http://auskalo.net/honi-buruz/");
            #endif
            #ifdef ANDROID
                CCApplication::sharedApplication()->openURLJNI("http://auskalo.net/honi-buruz/");
            #endif
            break;
        default:
            break;
    }
    
}

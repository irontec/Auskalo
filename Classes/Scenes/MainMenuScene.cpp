//
//  MainMenuScene.cpp
//  TrivialJokoa
//
//  Created by iker on 12/12/12.
//
//

#include "MainMenuScene.h"
#include "../Layers/MainMenuLayer.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/ImageManager.h"
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
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    addChild(fondo);
    
    Sprite *leftGrass = Sprite::create("grass_left.png");
    leftGrass->setAnchorPoint(Vec2(0,0));
    leftGrass->setPosition(VisibleRect::leftBottom());
    addChild(leftGrass);
    Sprite *rightGrass = Sprite::create("grass_right.png");
    rightGrass->setAnchorPoint(Vec2(1,0));
    rightGrass->setPosition(VisibleRect::rightBottom());
    addChild(rightGrass);
    
    Sprite *spriteLogo = Sprite::create("logo_opciones.png");
    spriteLogo->setPosition(Vec2(VisibleRect::center().x, (VisibleRect::top().y - (spriteLogo->getContentSize().height / 2) - 20)));
    addChild(spriteLogo);
    
    int mySize = VisibleRect::center().y / 3;
    
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("botoia_normal.png");
    
    Point anchorPoint = Vec2(0.5f, 0.5f);
    
    Label *pLabel1 = Label::createWithTTF("Jokatu", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    Label *pLabel2 = Label::createWithTTF("Sailkapena", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    Label *pLabel3 = Label::createWithTTF("Honi Buruz", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    
    // Button JOKATU
    auto pButton1 = SpriteButton::create(ImageManager::getImage("botoia_normal"), 0.9f, CC_CALLBACK_1(MainMenuScene::menuSelector, this));
    pButton1->setTag(1);
    pButton1->setAnchorPoint(anchorPoint);
    pButton1->setPosition(Vec2(VisibleRect::center().x, (mySize * 3) - texture->getContentSize().height /2));
    pLabel1->setPosition(Vec2(VisibleRect::center().x, (mySize * 3) - texture->getContentSize().height /2));
    
    // Button SAILKAPENA
    auto pButton2 = SpriteButton::create(ImageManager::getImage("botoia_normal"), 0.9f, CC_CALLBACK_1(MainMenuScene::menuSelector, this));
    pButton2->setTag(2);
    pButton2->setAnchorPoint(anchorPoint);
    pButton2->setPosition(Vec2(VisibleRect::center().x, (mySize * 2) - texture->getContentSize().height /2));
    pLabel2->setPosition(Vec2(VisibleRect::center().x, (mySize * 2) - texture->getContentSize().height /2));
    
    // Button HONI BURUZ
    auto pButton3 = SpriteButton::create(ImageManager::getImage("botoia_normal"), 0.9f, CC_CALLBACK_1(MainMenuScene::menuSelector, this));
    pButton3->setTag(3);
    pButton3->setAnchorPoint(anchorPoint);
    pButton3->setPosition(Vec2(VisibleRect::center().x, mySize - texture->getContentSize().height /2));
    pLabel3->setPosition(Vec2(VisibleRect::center().x, mySize - texture->getContentSize().height /2));
    
    addChild(pButton1);
    addChild(pLabel1);
    addChild(pButton2);
    addChild(pLabel2);
    addChild(pButton3);
    addChild(pLabel3);
    
	return true;
}

void MainMenuScene::menuSelector(Ref* pSender)
{
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene;
    SpriteButton *selectedOpt = (SpriteButton*) pSender;
    Scene *scene;
    switch (selectedOpt->getTag()) {
        case 1:
            scene = CategoryScene::create();
            tscene = TransitionFade::create(0.5, scene);
            pDirector->replaceScene(tscene);
            break;
        case 2:
            scene = ProfilaScene::create();
            tscene = TransitionFade::create(0.5, scene);
            pDirector->replaceScene(tscene);
            break;
        case 3:
            Application::getInstance()->openURL("http://auskalo.net/honi-buruz/");
            break;
        default:
            break;
    }
    
}

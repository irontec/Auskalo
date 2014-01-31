//
//  BukatutaScene.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 11/01/13.
//
//

#include "BukatutaScene.h"
#include "../Layers/BukatutaLayer.h"
#include "CCNodeLoaderLibrary.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/VisibleRect.h"
#include "Globals.h"
#include "../Scenes/CategoryScene.h"

BukatutaScene::~BukatutaScene()
{
    
}

BukatutaScene::BukatutaScene()
{
    
}


// on "init" you need to initialize your instance
bool BukatutaScene::init()
{
    
    CCSprite *fondo = CCSprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    
    addChild(fondo, 0);
    
    int mySize = VisibleRect::center().y / 3;
    
    // Button JARRAITU
    CCTexture2D *texture3 = CCTextureCache::sharedTextureCache()->addImage("botoia_normal.png");
    SpriteButton *pButton3 = SpriteButton::create(texture3 ,this, callfuncO_selector(BukatutaScene::buttonJarraitu), 1.0f);
    pButton3->setTag(3);
    pButton3->setPosition(ccp(VisibleRect::center().x, mySize - (texture3->getContentSize().height / 2)));
    pButton3->setAnchorPoint(ccp(0.5f, 0.5f));
    CCLabelTTF *pLabel3 = CCLabelTTF::create("Jarraitu", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    pLabel3->setPosition(ccp(VisibleRect::center().x, mySize - (texture3->getContentSize().height / 2)));
    addChild(pButton3);
    addChild(pLabel3);
    
    std::string *name = new std::string("bukatuduzu_");
    std::string *nameIzar = new std::string();
    
    int kalifikazioa = multzoa->kalifikazioa;
    
    int kaliFiveStart = kFiveStart;
    int kaliFourStart = kFourStart;
    int kaliThreeStart = kThreeStart;
    int kaliTwoStart = kTwoStart;
    
    float bonus;
    
    switch (multzoa->zailtasuna) {
        case 1:
            name->append("erraza.png");
            bonus = 1;
            break;
        case 2:
            name->append("normala.png");
            bonus = 1.5;
            break;
        case 3:
            name->append("zaila.png");
            bonus = 2;
            break;
        default:
            break;
    }

    CCLog("kaliFiveStart = %f, kFourStart = %f, kThreeStart = %f, kTwoStart = %f", (kaliFiveStart * bonus), (kaliFourStart * bonus), (kaliThreeStart * bonus), (kaliTwoStart * bonus));
    
    if (kalifikazioa >= (kFiveStart * bonus)) {
        nameIzar->append("5_izar.png");
    } else if(kalifikazioa >= (kFourStart * bonus)) {
        nameIzar->append("4_izar.png");
    } else if(kalifikazioa >= (kThreeStart * bonus)) {
        nameIzar->append("3_izar.png");
    } else if(kalifikazioa > (kTwoStart * bonus)) {
        nameIzar->append("2_izar.png");
    } else if (kalifikazioa <= (kTwoStart * bonus)) {
        nameIzar->append("1_1_izar.png");
    }
    
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(name->c_str());
    pImage = CCSprite::createWithTexture(texture);
    pImage->setPosition(ccp(fondo->getPosition().x, VisibleRect::top().y - (texture->getContentSize().height / 2) - 20));
    
    CCTexture2D* textureIzar = CCTextureCache::sharedTextureCache()->addImage(nameIzar->c_str());
    pStars = CCSprite::createWithTexture(textureIzar);
    pStars->setAnchorPoint(ccp(0.5, 1));
    pStars->setPosition(ccp(VisibleRect::center().x, pImage->getPosition().y - pImage->getContentSize().height / 2 - 14));
    
    addChild(pImage);
    addChild(pStars);
    
    delete name;
    delete nameIzar;
    
	return true;
}

void BukatutaScene::buttonJarraitu(CCObject* pSender)
{
    CategoryScene *scene = CategoryScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}


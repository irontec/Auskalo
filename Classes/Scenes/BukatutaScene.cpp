//
//  BukatutaScene.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 11/01/13.
//
//

#include "BukatutaScene.h"
#include "../Layers/BukatutaLayer.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/ImageManager.h"
#include "../Helpers/Globals.h"
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
    
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    
    addChild(fondo, 0);
    
    int mySize = VisibleRect::center().y / 3;
    
    // Button JARRAITU
    Texture2D *texture3 = Director::getInstance()->getTextureCache()->addImage("botoia_normal.png");
    auto pButton3 = SpriteButton::create(ImageManager::getImage("botoia_normal"), 1.0f, CC_CALLBACK_1(BukatutaScene::buttonJarraitu, this));
    pButton3->setTag(3);
    pButton3->setPosition(Vec2(VisibleRect::center().x, mySize - (texture3->getContentSize().height / 2)));
    pButton3->setAnchorPoint(Vec2(0.5f, 0.5f));
    Label *pLabel3 = Label::createWithTTF("Jarraitu", "fonts/PT_Sans-Web-Bold.ttf", 30.0);
    pLabel3->setPosition(Vec2(VisibleRect::center().x, mySize - (texture3->getContentSize().height / 2)));
    addChild(pButton3);
    addChild(pLabel3);
    
    std::string *name = new std::string("bukatuduzu_");
    std::string *nameIzar = new std::string();
    
    int kalifikazioa = multzoa->kalifikazioa;
    
    int kaliFiveStart = kFiveStart;
    int kaliFourStart = kFourStart;
    int kaliThreeStart = kThreeStart;
    int kaliTwoStart = kTwoStart;
    
    float bonus = 1;
    
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

    log("kaliFiveStart = %f, kFourStart = %f, kThreeStart = %f, kTwoStart = %f", (kaliFiveStart * bonus), (kaliFourStart * bonus), (kaliThreeStart * bonus), (kaliTwoStart * bonus));
    
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
    
    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(name->c_str());
    pImage = Sprite::createWithTexture(texture);
    pImage->setPosition(Vec2(fondo->getPosition().x, VisibleRect::top().y - (texture->getContentSize().height / 2) - 20));
    
    Texture2D* textureIzar = Director::getInstance()->getTextureCache()->addImage(nameIzar->c_str());
    pStars = Sprite::createWithTexture(textureIzar);
    pStars->setAnchorPoint(Vec2(0.5, 1));
    pStars->setPosition(Vec2(VisibleRect::center().x, pImage->getPosition().y - pImage->getContentSize().height / 2 - 14));
    
    addChild(pImage);
    addChild(pStars);
    
    delete name;
    delete nameIzar;
    
	return true;
}

void BukatutaScene::buttonJarraitu(Ref* pSender)
{
    CategoryScene *scene = CategoryScene::create();
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}


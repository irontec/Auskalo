//
//  BukatutaLayer.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 11/01/13.
//
//

#include "BukatutaLayer.h"
#include "../Scenes/CategoryScene.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/Globals.h"

BukatutaLayer::BukatutaLayer()
{
    
}

BukatutaLayer::~BukatutaLayer()
{
    
}

void BukatutaLayer::onEnter()
{
    std::string *name = new std::string("bukatuduzu_");
    
    switch (multzoa->zailtasuna) {
        case 1:
            name->append("erraza.png");
            break;
        case 2:
            name->append("normala.png");
            break;
        case 3:
            name->append("zaila.png");
            break;
            
        default:
            break;
    }
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage(name->c_str());
    pImage->setTexture(t);
    
    delete name;
    
    std::string *nameIzar = new std::string();
    
    int kalifikazioa = multzoa->kalifikazioa;
    
    int kaliFiveStart = kFiveStart;
    int kaliFourStart = kFourStart;
    int kaliThreeStart = kThreeStart;
    int kaliTwoStart = kTwoStart;
    
    CCLog("kaliFiveStart = %d, kFourStart = %d, kThreeStart = %d, kTwoStart = %d", kaliFiveStart, kaliFourStart, kaliThreeStart, kaliTwoStart);
    
    float bonus;
    switch (multzoa->zailtasuna) {
        case 2:
            bonus = 1.5;
            break;
        case 3:
            bonus = 2;
            break;
        default:
            bonus = 1;
            break;
    }
    
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
    
    t = CCTextureCache::sharedTextureCache()->addImage(name->c_str());
    pStars = CCSprite::createWithTexture(t);
    pStars->setAnchorPoint(ccp(0.5, 1));
    pStars->setPosition(ccp(VisibleRect::center().x, pImage->getPosition().y - pImage->getContentSize().height / 2 - 14));
    
    addChild(pStars);
    
    //delete nameIzar;
    
    CCLayer::onEnter();
}


bool BukatutaLayer::init()
{
    
    return true;
}

void BukatutaLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    pImage = (CCSprite*) pNode->getChildByTag(100);
}


SEL_MenuHandler BukatutaLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler BukatutaLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buttonClicked", BukatutaLayer::buttonClicked);
    return NULL;
}

SEL_CallFuncN BukatutaLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

void BukatutaLayer::buttonClicked(CCObject *pSender, CCControlEvent pCCControlEvent) {
    
    CategoryScene *scene = CategoryScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

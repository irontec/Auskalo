//
//  BukatutaLayer.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 11/01/13.
//
//

#include "BukatutaLayer.h"
#include "CategoryScene.h"
#include "VisibleRect.h"
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
    
    if (kalifikazioa >= kFiveStart) {
        nameIzar->append("5_izar.png");
    } else if(kalifikazioa >= kFourStart) {
        nameIzar->append("4_izar.png");
    } else if(kalifikazioa >= kThreeStart) {
        nameIzar->append("3_izar.png");
    } else if(kalifikazioa >= kTwoStart) {
        nameIzar->append("2_izar.png");
    } else {
        nameIzar->append("1_izar.png");
    }
    
    t = CCTextureCache::sharedTextureCache()->addImage(name->c_str());
    pStars = CCSprite::createWithTexture(t);
    pStars->setAnchorPoint(ccp(0.5, 1));
    pStars->setPosition(ccp(VisibleRect::center().x, pImage->getPosition().y - pImage->getContentSize().height / 2 - 14));
    
    addChild(pStars);
    
    delete nameIzar;
    
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


SEL_MenuHandler BukatutaLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler BukatutaLayer::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buttonClicked", BukatutaLayer::buttonClicked);
    return NULL;
}


void BukatutaLayer::buttonClicked(CCObject *pSender, CCControlEvent pCCControlEvent) {
    
    CategoryScene *scene = CategoryScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

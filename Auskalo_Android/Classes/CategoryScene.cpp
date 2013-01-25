//
//  CategoryScene.cpp
//  TrivialJokoa
//
//  Created by iker on 13/12/12.
//
//

#include "CategoryScene.h"
#include "VisibleRect.h"
#include "CCScrollLayer.h"
#include "SQLHelper.h"
#include "MainMenuScene.h"
#include "LevelScene.h"
#include "SpriteButton.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

CategoryScene::~CategoryScene()
{
    
}

CategoryScene::CategoryScene()
{
    
}

bool CategoryScene::init()
{
    kategoriak = getKategoriak();
    kategoriak->retain();
    
    int pages = kategoriak->count() / 4;
    
    if (kategoriak->count() / 4 != 0) {
        pages++;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerGradient *pBglayer = CCLayerGradient::create(ccc4(154, 182, 0, 255), ccc4(133, 160, 0, 255));
    pBglayer->setContentSize(size);
    
    CCSprite *pHierbaLeft = CCSprite::create("grass_left.png");
    pHierbaLeft->setAnchorPoint(CCPointZero);
    pHierbaLeft->setPosition(VisibleRect::leftBottom());
    pBglayer->addChild(pHierbaLeft);
    
    CCSprite *pHierbaRight = CCSprite::create("grass_right.png");
    pHierbaRight->setAnchorPoint(ccp(1,0));
    pHierbaRight->setPosition(VisibleRect::rightBottom());
    pBglayer->addChild(pHierbaRight);
    
    CCSprite *pTitle = CCSprite::create("gaiak.png");
    pTitle->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - 75));
    pBglayer->addChild(pTitle);
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("go-back.png");
    SpriteButton *pBackButton = SpriteButton::create(t, this, callfuncO_selector(CategoryScene::backButton));
    pBackButton->setAnchorPoint(ccp(0, 1));
    pBackButton->setPosition(VisibleRect::leftTop());
    pBglayer->addChild(pBackButton);
    
    CCArray *pScrollLayers = CCArray::createWithCapacity(10);
    CCSize scrollLayerSize = CCSizeMake(size.width, pTitle->getPosition().y - pTitle->getContentSize().height/2 - 10);
    CCSize buttonSize = CCSizeMake(290, 54);
    
    
    //Calculate padding
    float tmp = scrollLayerSize.height - (4.0 * buttonSize.height) - VisibleRect::bottom().y + 25;
    float padding = (tmp / 8);
    
    int numberRows = kategoriak->count();
            
    for (int i = 0; i <= pages; i++) {
        
        CCPoint initialPosition = ccp(pTitle->getPosition().x, pTitle->getPosition().y - pTitle->getContentSize().height - 10);
        
        //CCLayerColor *pageLayer = CCLayerColor::create(ccc4(255, 0, 0, 255));
        CCLayer *pageLayer = CCLayer::create();
        pageLayer->setPosition(ccp(size.width/2, size.height/2));
        pageLayer->setContentSize(scrollLayerSize);
        
        if (numberRows == 0) {
            goto exit;
        }
            
        for (int j = 0; j < 4; j++) {
            
            numberRows--;
            
            int position = i*4+j;
            KategoriaModel *kategoria = (KategoriaModel*) kategoriak->objectAtIndex(position);
            
            //15 caracteres
            CCLabelTTF *label = CCLabelTTF::create(kategoria->izena.c_str(), "fonts/PT_Sans-Web-Regular.ttf", 15.0);
            
            CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("gaiak_button.png");
            SpriteButton *pButton = SpriteButton::create(t, this, callfuncO_selector(CategoryScene::loadLevels));
            
            pButton->setTag(position);
                        
            CCPoint buttonPoint = pageLayer->getPosition();
            pButton->setPosition(initialPosition);
            label->setPosition(ccp(initialPosition.x, initialPosition.y));
            pageLayer->addChild(pButton);
            pageLayer->addChild(label);
            
            if (numberRows == 0) {
                break;
            }
            
            initialPosition.y -= pButton->getContentSize().height + padding;
        }
        
        pScrollLayers->addObject(pageLayer);
        
        if (numberRows == 0) {
            goto exit;
        }
    }
    
exit:
    
    CCScrollLayer *pScrollLayer = new CCScrollLayer();

    pScrollLayer->initWithLayers(pScrollLayers, 0);
    pScrollLayer->setPagesIndicatorPosition(ccp(size.width/2, scrollLayerSize.height - ((4.0 * buttonSize.height) + (padding * 5.0))));
    
    pBglayer->addChild(pScrollLayer);
   
    addChild(pBglayer);
    
	return true;
}

CCArray* CategoryScene::getKategoriak()
{
    SQLHelper *sqlHelper = SQLHelper::sharedInstance();
    return sqlHelper->queryKategoriak();
}

void CategoryScene::backButton(CCObject *sender, CCControlEvent controlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    MainMenuScene *scene = MainMenuScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void CategoryScene::loadLevels(CCObject *sender, CCControlEvent controlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    CCControlButton *button = (CCControlButton*) sender;
    
    KategoriaModel *kategoria = (KategoriaModel*) kategoriak->objectAtIndex(button->getTag());
    kategoriak->release();
    
    LevelScene *scene = LevelScene::create(*kategoria);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
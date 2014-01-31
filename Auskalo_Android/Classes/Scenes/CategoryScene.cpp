//
//  CategoryScene.cpp
//  TrivialJokoa
//
//  Created by iker on 13/12/12.
//
//

#include "../Scenes/CategoryScene.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/SQLHelper.h"
#include "../Scenes/MainMenuScene.h"
#include "../Scenes/LevelScene.h"
#include "../Helpers/SpriteButton.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

CategoryScene::~CategoryScene()
{
    CC_SAFE_RELEASE(m_pBolasSprites);
}

CategoryScene::CategoryScene()
{
    m_pBolasSprites = CCArray::create();
    m_pBolasSprites->retain();
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
    
    CCLayer *pBglayer = CCLayer::create();
    pBglayer->setContentSize(size);
    
    CCSprite *fondo = CCSprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    this->addChild(fondo);
    
    CCSprite *leftGrass = CCSprite::create("grass_left.png");
    leftGrass->setAnchorPoint(ccp(0,0));
    leftGrass->setPosition(VisibleRect::leftBottom());
    addChild(leftGrass);
    CCSprite *rightGrass = CCSprite::create("grass_right.png");
    rightGrass->setAnchorPoint(ccp(1,0));
    rightGrass->setPosition(VisibleRect::rightBottom());
    addChild(rightGrass);
    
    CCSprite *pTitle = CCSprite::create("gaiak.png");
    pTitle->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - 90));
    pBglayer->addChild(pTitle);
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("go-back.png");
    SpriteButton *pBackButton = SpriteButton::create(t, this, callfuncO_selector(CategoryScene::backButton));
    pBackButton->setAnchorPoint(ccp(0, 1));
    pBackButton->setPosition(ccp(VisibleRect::left().x, (VisibleRect::top().y - 4)));
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
            
            CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("botoia_normal.png");
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
    pScrollLayer->setPagesIndicatorPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y + 10));
    pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(0);
    pBglayer->addChild(pScrollLayer);
   
    addChild(pBglayer);
    
	return true;
}

void CategoryScene::scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page)
{
    if (sender->getShowPagesIndicator())
    {
        CCObject *item;
        CCARRAY_FOREACH(m_pBolasSprites, item)
        {
            CCSprite *sprite = (CCSprite*)item;
            removeChild(sprite, false);
        }
        
        m_pBolasSprites->removeAllObjects();
        
        int totalScreens = sender->getTotalScreens();
        int currentScreen = sender->getCurrentScreen();
        
        CCPoint pageIndicatorPosition = sender->getPagesIndicatorPosition();
        
        // Prepare Points Array
        float n = (float)totalScreens; //< Total points count in CGFloat.
        float pY = pageIndicatorPosition.y; //< Points y-coord in parent coord sys.
        float d = 30.0f; //< Distance between points.
        
        CCPoint* points = new CCPoint[totalScreens];
        for (int i = 0; i < totalScreens; ++i)
        {
            float pX = pageIndicatorPosition.x + d * ((float)i - 0.5f*(n-1.0f));
            points[i] = ccp(pX, pY);
        }
        
        for (int i = 0; i < totalScreens; i++) {
            
            CCSprite *bola;
            
            if (i == currentScreen) {
                bola = CCSprite::create("bola_roja.png");
            } else {
                bola = CCSprite::create("bola_gris.png");
            }
            
            bola->setPosition(points[i]);
            m_pBolasSprites->addObject(bola);
        }
        
        delete [] points;
        
        CCARRAY_FOREACH(m_pBolasSprites, item)
        {
            CCSprite *sprite = (CCSprite*)item;
            addChild(sprite);
        }
    }
}


CCArray* CategoryScene::getKategoriak()
{
    SQLHelper *sqlHelper = SQLHelper::sharedInstance();
    return sqlHelper->queryKategoriak();
}

void CategoryScene::backButton(CCObject *sender, CCControlEvent controlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    MainMenuScene *scene = MainMenuScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void CategoryScene::loadLevels(CCObject *sender, CCControlEvent controlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    CCControlButton *button = (CCControlButton*) sender;
    
    KategoriaModel *kategoria = (KategoriaModel*) kategoriak->objectAtIndex(button->getTag());
    kategoriak->release();
    
    LevelScene *scene = LevelScene::create(*kategoria);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
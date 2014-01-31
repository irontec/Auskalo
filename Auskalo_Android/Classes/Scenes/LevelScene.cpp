//
//  LevelSelectorScene.cpp
//  TrivialJokoa
//
//  Created by iker on 13/12/12.
//
//

#include "../Scenes/LevelScene.h"
#include "../Helpers/VisibleRect.h"
#include "../Scenes/CategoryScene.h"
#include "../Scenes/GalderaScene.h"
#include "../Helpers/SQLHelper.h"
#include "SimpleAudioEngine.h"
#include "../Helpers/Globals.h"

#define kGridElements 4

using namespace CocosDenshion;

LevelScene::~LevelScene()
{
   CC_SAFE_RELEASE(m_pBolasSprites);
}

LevelScene::LevelScene()
{
    m_pBolasSprites = CCArray::create();
    m_pBolasSprites->retain();
}

/*int calculateFontSizeForString() {
    int fontSize = 30; // it seems to be the biggest font we can use
    while (--fontSize > 0) {
        CGSize size = [string sizeWithFont:[UIFont fontWithName:usedFontName size:fontSize]];
        if (size.width <= 480 && size.height <= 360)
            break;
    }
    
    return fontSize;
}*/

// on "init" you need to initialize your instance
bool LevelScene::init()
{
    SQLHelper *pSQLHelper = SQLHelper::sharedInstance();
    multzoak = pSQLHelper->queryMultzoak(kategoria.idKategoria);
    multzoak->retain();
    
    int numElements = multzoak->count();
    
    int numPages = numElements / kGridElements;
   
    if (numPages == 0)
        numPages = 1;
    
    if (numElements % kGridElements != 0)
        numPages++;
    
    m_pButtonsArray = CCArray::create();
    
    CCSize screenSize = VisibleRect::getVisibleRect().size;
    
    CCLayer *pBglayer = CCLayer::create();
    pBglayer->setPosition(ccp(VisibleRect::leftBottom().x, VisibleRect::leftBottom().y));
    pBglayer->setContentSize(screenSize);
    
    CCSprite *fondo = CCSprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    
    addChild(fondo);
    
    CCLabelTTF *pTitle = CCLabelTTF::create(kategoria.izena.c_str(), "font/CarterOne.ttf", 20.0);
    pTitle->setDimensions(CCSizeMake(150, 50));
    pTitle->setHorizontalAlignment(kCCTextAlignmentCenter);
    pTitle->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    pTitle->setColor(ccc3(0, 0, 0));
    pTitle->setPosition(ccp(screenSize.width/2, VisibleRect::top().y - 40));
    
    pBglayer->addChild(pTitle);
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("go-back.png");
    SpriteButton *pBackButton = SpriteButton::create(t, this, callfuncO_selector(LevelScene::backButton));
    pBackButton->setAnchorPoint(ccp(0, 1));
    pBackButton->setPosition(ccp(0, (VisibleRect::top().y - 4)));
    pBglayer->addChild(pBackButton);
    
    CCArray *pScrollLayers = CCArray::createWithCapacity(numPages);
    
    CCSize areaLayerSize = CCSizeMake(140, 162);
    
    CCSize gridLayerSize = CCSizeMake(screenSize.width, pTitle->getPosition().y - pTitle->getContentSize().height / 2);
    
    MultzoaModel *multzoa;
    int count = 0;
    
    for (int i = 1; i <= numPages; i++) {
        
        CCLayer *lContainer = CCLayer::create();
        
        CCLayer *lContainerSubView = CCLayer::create();
        lContainerSubView->setContentSize(CCSizeMake(gridLayerSize.width, gridLayerSize.height));
        
        CCPoint layerPosition[] = {
            ccp((gridLayerSize.width / 4) * 1, (gridLayerSize.height / 4) * 3), // up-left
            ccp((gridLayerSize.width / 4) * 1, (gridLayerSize.height / 4) * 1), // down-left
            ccp((gridLayerSize.width / 4) * 3, (gridLayerSize.height / 4) * 3), // up-right
            ccp((gridLayerSize.width / 4) * 3, (gridLayerSize.height / 4) * 1)  // down-right
        };
        
        for (int j = 0; j < 4; j++) {
            if (count >= multzoak->count()) {
                break;
            }
            multzoa = (MultzoaModel*) multzoak->objectAtIndex(count);
            
            CCLayer *pAreaLayer = CCLayer::create();
            pAreaLayer->setContentSize(CCSizeMake(gridLayerSize.width / 2, gridLayerSize.height / 2));
            pAreaLayer->setPosition(layerPosition[j]);
            pAreaLayer->setAnchorPoint(ccp(0.5,0.5));
            pAreaLayer->ignoreAnchorPointForPosition(false);
            
            SpriteButton *pButton = getButton(multzoa);
            CCPoint buttonPosition = ccp(pAreaLayer->getContentSize().width/2, pAreaLayer->getContentSize().height/2 + 10);
            pButton->setPosition(buttonPosition);
            pButton->setAnchorPoint(ccp(0.5f,0.5f));
            pButton->setTag(count);
            pButton->ignoreAnchorPointForPosition(false);
            pAreaLayer->addChild(pButton);
            
            CCLabelTTF *pMultzoaTitle = CCLabelTTF::create(multzoa->izena.c_str(), "fonts/PT_Sans-Web-Regular.ttf", 14.0);
            pMultzoaTitle->setColor(ccc3(0, 0, 0));
            pMultzoaTitle->setContentSize(pAreaLayer->getContentSize());
            pMultzoaTitle->setPosition(ccp(pAreaLayer->getContentSize().width/2, 30));
            pMultzoaTitle->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
            pMultzoaTitle->setHorizontalAlignment(kCCTextAlignmentCenter);
            pAreaLayer->addChild(pMultzoaTitle);
            
            lContainerSubView->addChild(pAreaLayer);
            count++;
        }
        
        lContainer->addChild(lContainerSubView);
        pScrollLayers->addObject(lContainer);
    }
    
    CCScrollLayer *pScrollLayer = new CCScrollLayer();
    pScrollLayer->initWithLayers(pScrollLayers, 0);
    pScrollLayer->setPosition(ccp(0, 0));
    pScrollLayer->setPagesIndicatorPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y + 10));
    //pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(numPages - (numPages - 1));
    CCDelayTime *readyDelay = new CCDelayTime();
    readyDelay->initWithDuration(0.7f);
    CCFiniteTimeAction *readySequence = CCSequence::create(readyDelay, NULL);
    runAction(readySequence);
    pScrollLayer->moveToPage(0);
    pBglayer->addChild(pScrollLayer);
    
    addChild(pBglayer);
    
	return true;
}

void LevelScene::scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page)
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

SpriteButton* LevelScene::getButton(MultzoaModel *multzoa)
{
    std::string *name = new std::string("pack-");
    
    switch (multzoa->zailtasuna) {
        case 1:
            name->append("erraza-");
            break;
        case 2:
            name->append("normala-");
            break;
        case 3:
            name->append("zaila-");
            break;
            
        default:
            break;
    }
    
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
    
    if (multzoa->bukatuta == 1) {
        int kalifikazioa = multzoa->kalifikazioa;
        if (kalifikazioa >= kFiveStart * bonus) {
            name->append("5star.png");
        } else if(kalifikazioa >= kFourStart * bonus) {
            name->append("4star.png");
        } else if(kalifikazioa >= kThreeStart * bonus) {
            name->append("3star.png");
        } else if(kalifikazioa > kTwoStart * bonus) {
            name->append("2star.png");
        } else if(kalifikazioa <= kTwoStart * bonus){
            name->append("1star.png");
        }
    } else if(multzoa->currentGaldera > 1) {
       name->append("jokoan.png");
    } else {
       name->append("0star.png");
    }
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage(name->c_str());
    SpriteButton *pButton = SpriteButton::create(t, this, callfuncO_selector(LevelScene::levelSelected));
    delete name;
    return pButton;
}

void LevelScene::levelSelected(CCObject *o)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    SpriteButton *b = (SpriteButton*) o;
    int position = b->getTag();
    MultzoaModel *multzoa = (MultzoaModel*) multzoak->objectAtIndex(position);
    if (multzoa->bukatuta == 1) {
        multzoa->bukatuta = 0;
        multzoa->kalifikazioa = 0;
        multzoa->currentGaldera = 1;
        SQLHelper *pSQLHelper = SQLHelper::sharedInstance();
        pSQLHelper->resetLevel(multzoa);
    }
    GalderaScene *scene = GalderaScene::create(multzoa);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void LevelScene::backButton(CCObject *sender, CCControlEvent controlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    CategoryScene *scene = CategoryScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
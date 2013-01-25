//
//  LevelSelectorScene.cpp
//  TrivialJokoa
//
//  Created by iker on 13/12/12.
//
//

#include "LevelScene.h"
#include "VisibleRect.h"
#include "CCScrollLayer.h"
#include "CategoryScene.h"
#include "GalderaScene.h"
#include "SQLHelper.h"
#include "SimpleAudioEngine.h"
#include "../Helpers/Globals.h"

#define kGridElements 4

using namespace CocosDenshion;

LevelScene::~LevelScene()
{
   
}

LevelScene::LevelScene()
{
    
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
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerGradient *pBglayer = CCLayerGradient::create(ccc4(154, 182, 0, 255), ccc4(133, 160, 0, 255));
    pBglayer->setContentSize(screenSize);
    
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
    pBackButton->setPosition(VisibleRect::leftTop());
    pBglayer->addChild(pBackButton);
    
    CCArray *pScrollLayers = CCArray::createWithCapacity(numPages);
    
    CCSize areaLayerSize = CCSizeMake(140, 162);
    
    CCLayer *pGridLayer;
    
    CCSize gridLayerSize = CCSizeMake(screenSize.width, pTitle->getPosition().y - pTitle->getContentSize().height / 2);
    
    float tmp = gridLayerSize.width - ((areaLayerSize.width * (kGridElements / 2)));
    float padding = tmp / (kGridElements - 1);
    
    MultzoaModel *multzoa;
    int count = 0;
    
    for (int i = 1; i <= numPages; i++) {
        
        pGridLayer = CCLayer::create();
        pGridLayer->setContentSize(gridLayerSize);
        
        CCPoint initialPosition = ccp(padding, gridLayerSize.height - areaLayerSize.height);
        
        if (numElements == 0)
            goto exit;
        
        for (int y = 0; y < (kGridElements/2); y++) {
            
            for (int x = 0; x < (kGridElements/2); x++) {
                
                multzoa = (MultzoaModel*) multzoak->objectAtIndex(count);
                
                CCLayer *pAreaLayer = CCLayer::create();
                pAreaLayer->setContentSize(areaLayerSize);
                pAreaLayer->setPosition(initialPosition);
                
                SpriteButton *pButton = getButton(multzoa);
                                
                CCSize buttonSize = CCSizeMake(140, 162);
                CCPoint buttonPosition = ccp(areaLayerSize.width/2, areaLayerSize.height/2);
                pButton->setPosition(buttonPosition);
                pButton->setTag(count);
                pAreaLayer->addChild(pButton);
                
                //CCLabelTTF *pMultzoaTitle = CCLabelTTF::create(multzoa->izena.c_str(), "font/CarterOne.ttf", 16.0);
                CCLabelTTF *pMultzoaTitle = CCLabelTTF::create(multzoa->izena.c_str(), "fonts/PT_Sans-Web-Regular.ttf", 16.0);
                pMultzoaTitle->setColor(ccc3(0, 0, 0));
                pMultzoaTitle->setPosition(ccp(buttonPosition.x, areaLayerSize.height/2 - 66));
                pAreaLayer->addChild(pMultzoaTitle);
                
                pGridLayer->addChild(pAreaLayer);
                
                initialPosition.x += areaLayerSize.width + padding;
                
                numElements--;
                count++;
                
                if (numElements == 0)
                    goto exit;
                
            }
            
            CCPoint newPosition = ccp(padding, initialPosition.y - areaLayerSize.height - padding);
            
            initialPosition = newPosition;
            
        }
        
        exit:
        
        pScrollLayers->addObject(pGridLayer);
        
        if (numElements == 0)
            goto exit2;

    }
    
    exit2:
    
    CCScrollLayer *pScrollLayer = new CCScrollLayer();
    pScrollLayer->initWithLayers(pScrollLayers, 0);
    //pScrollLayer->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, 100));
    //pScrollLayer->setPosition(ccp(VisibleRect::getVisibleRect().size.width / 2, pTitle->getPosition().y));
    pScrollLayer->setPagesIndicatorPosition(ccp(screenSize.width/2, VisibleRect::bottom().y + 25));
    
    pBglayer->addChild(pScrollLayer);

    
    addChild(pBglayer);
    
	return true;
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
    
   if (multzoa->bukatuta == 1) {
        int kalifikazioa = multzoa->kalifikazioa;
        if (kalifikazioa >= kFiveStart) {
            name->append("5star.png");
        } else if(kalifikazioa >= kFourStart) {
            name->append("4star.png");
        } else if(kalifikazioa >= kThreeStart) {
            name->append("3star.png");
        } else if(kalifikazioa >= kTwoStart) {
            name->append("2star.png");
        } else {
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
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
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
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    CategoryScene *scene = CategoryScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
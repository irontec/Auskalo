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
#include "../Helpers/ImageManager.h"

#define kGridElements 4

using namespace CocosDenshion;

LevelScene::~LevelScene()
{
}

LevelScene::LevelScene()
{
    m_pBolasSprites = new Vector<Sprite*>();
}

// on "init" you need to initialize your instance
bool LevelScene::init()
{
    SQLHelper *pSQLHelper = SQLHelper::getInstance();
    multzoak = pSQLHelper->queryMultzoak(kategoria->idKategoria);
    
    int numElements = (int) multzoak->size();
    
    int numPages = numElements / kGridElements;
   
    if (numPages == 0)
        numPages = 1;
    
    if (numElements % kGridElements != 0)
        numPages++;
    
    m_pButtonsArray = new Vector<Ref*>();
    
    Size screenSize = VisibleRect::getVisibleRect().size;
    
    Layer *pBglayer = Layer::create();
    pBglayer->setPosition(Vec2(VisibleRect::leftBottom().x, VisibleRect::leftBottom().y));
    pBglayer->setContentSize(screenSize);
    
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    
    addChild(fondo);
    
    auto pTitle = Label::createWithTTF(kategoria->izena.c_str(), "fonts/CarterOne.ttf", 20.0f);
    pTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    pTitle->setVerticalAlignment(TextVAlignment::CENTER);
    pTitle->setColor(Color3B(0, 0, 0));
    pTitle->setPosition(Vec2(screenSize.width/2, VisibleRect::top().y - 40));
    
    pBglayer->addChild(pTitle);
    
    auto pBackButton = SpriteButton::create(ImageManager::getImage("go-back"), 1.0f, CC_CALLBACK_1(LevelScene::backButton, this));
    pBackButton->setAnchorPoint(Vec2(0, 1));
    //pBackButton->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y));
    pBackButton->setPosition(Vec2(0, VisibleRect::top().y));
    pBglayer->addChild(pBackButton);
    
    auto pScrollLayers = new Vector<Layer*>(numPages);
    
    Size gridLayerSize = Size(screenSize.width, pTitle->getPosition().y - pTitle->getContentSize().height / 2);
    
    MultzoaModel *multzoa;
    int count = 0;
    
    for (int i = 1; i <= numPages; i++) {
        
        Layer *lContainer = Layer::create();
        
        Layer *lContainerSubView = Layer::create();
        lContainerSubView->setContentSize(Size(gridLayerSize.width, gridLayerSize.height));
        
        Point layerPosition[] = {
            Vec2((gridLayerSize.width / 4) * 1, (gridLayerSize.height / 4) * 3), // up-left
            Vec2((gridLayerSize.width / 4) * 1, (gridLayerSize.height / 4) * 1), // down-left
            Vec2((gridLayerSize.width / 4) * 3, (gridLayerSize.height / 4) * 3), // up-right
            Vec2((gridLayerSize.width / 4) * 3, (gridLayerSize.height / 4) * 1)  // down-right
        };
        
        for (int j = 0; j < 4; j++) {
            if (count >= multzoak->size()) {
                break;
            }
            multzoa = (MultzoaModel*) multzoak->at(count);
            
            Layer *pAreaLayer = Layer::create();
            pAreaLayer->setContentSize(Size(gridLayerSize.width / 2, gridLayerSize.height / 2));
            pAreaLayer->setPosition(layerPosition[j]);
            pAreaLayer->setAnchorPoint(Vec2(0.5,0.5));
            pAreaLayer->setIgnoreAnchorPointForPosition(false);
            
            SpriteButton *pButton = getButton(multzoa);
            Point buttonPosition = Vec2(pAreaLayer->getContentSize().width/2, pAreaLayer->getContentSize().height/2 + 10);
            pButton->setPosition(buttonPosition);
            pButton->setAnchorPoint(Vec2(0.5f,0.5f));
            pButton->setTag(count);
            pButton->setIgnoreAnchorPointForPosition(false);
            pAreaLayer->addChild(pButton);
            
            Label *pMultzoaTitle = Label::createWithTTF(multzoa->izena.c_str(), "fonts/PT_Sans-Web-Regular.ttf", 14.0);
            pMultzoaTitle->setColor(Color3B(0, 0, 0));
            pMultzoaTitle->setContentSize(pAreaLayer->getContentSize());
            pMultzoaTitle->setPosition(Vec2(pAreaLayer->getContentSize().width/2, 30));
            pMultzoaTitle->setVerticalAlignment(TextVAlignment::CENTER);
            pMultzoaTitle->setHorizontalAlignment(TextHAlignment::CENTER);
            pAreaLayer->addChild(pMultzoaTitle);
            
            lContainerSubView->addChild(pAreaLayer);
            count++;
        }
        
        lContainer->addChild(lContainerSubView);
        pScrollLayers->pushBack(lContainer);
    }
    
    Size scollFrameSize = Director::getInstance()->getWinSize();
    pScrollLayer = ScrollLayer::nodeWithLayers(pScrollLayers, 0);
    pScrollLayer->setContentSize(scollFrameSize);
    pScrollLayer->setPosition(Vec2(0, 0));
    pScrollLayer->setPagesIndicatorPosition(Vec2(VisibleRect::center().x, VisibleRect::bottom().y + 10));
    pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(0);
    DelayTime *readyDelay = new DelayTime();
    readyDelay->initWithDuration(0.7f);
    FiniteTimeAction *readySequence = Sequence::create(readyDelay, NULL);
    runAction(readySequence);
    pBglayer->addChild(pScrollLayer);
    
    addChild(pBglayer);
    
	return true;
}

void LevelScene::scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page)
{
    if (sender->getShowPagesIndicator())
    {
        for (auto sprite: *m_pBolasSprites) {
            removeChild(sprite, false);
        }
        
        m_pBolasSprites->clear();
        
        int totalScreens = sender->getTotalScreens();
        int currentScreen = sender->getCurrentScreen();
        
        Point pageIndicatorPosition = sender->getPagesIndicatorPosition();
        
        // Prepare Points Array
        float n = (float)totalScreens; //< Total points count in CGFloat.
        float pY = pageIndicatorPosition.y; //< Points y-coord in parent coord sys.
        float d = 30.0f; //< Distance between points.
        
        Point* points = new Point[totalScreens];
        for (int i = 0; i < totalScreens; ++i)
        {
            float pX = pageIndicatorPosition.x + d * ((float)i - 0.5f*(n-1.0f));
            points[i] = Vec2(pX, pY);
        }
        
        for (int i = 0; i < totalScreens; i++) {
            
            Sprite *bola;
            
            if (i == currentScreen) {
                bola = Sprite::create("bola_roja.png");
            } else {
                bola = Sprite::create("bola_gris.png");
            }
            
            bola->setPosition(points[i]);
            m_pBolasSprites->pushBack(bola);
        }
        
        delete [] points;
        
        for (auto sprite: *m_pBolasSprites) {
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
            name->append("5star");
        } else if(kalifikazioa >= kFourStart * bonus) {
            name->append("4star");
        } else if(kalifikazioa >= kThreeStart * bonus) {
            name->append("3star");
        } else if(kalifikazioa > kTwoStart * bonus) {
            name->append("2star");
        } else if(kalifikazioa <= kTwoStart * bonus){
            name->append("1star");
        }
    } else if(multzoa->currentGaldera > 1) {
       name->append("jokoan");
    } else {
       name->append("0star");
    }
    
    auto pButton = SpriteButton::create(ImageManager::getImage(name->c_str()), 1.0f, CC_CALLBACK_1(LevelScene::levelSelected, this));
    delete name;
    return pButton;
}

void LevelScene::levelSelected(Ref* sender)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    SpriteButton *b = (SpriteButton*) sender;
    int position = b->getTag();
    MultzoaModel *multzoa = (MultzoaModel*) multzoak->at(position);
    if (multzoa->bukatuta == 1) {
        multzoa->bukatuta = 0;
        multzoa->kalifikazioa = 0;
        multzoa->currentGaldera = 1;
        SQLHelper *pSQLHelper = SQLHelper::getInstance();
        pSQLHelper->resetLevel(multzoa);
    }
    GalderaScene *scene = GalderaScene::create(multzoa);
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void LevelScene::backButton(Ref* sender)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    CategoryScene *scene = CategoryScene::create();
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

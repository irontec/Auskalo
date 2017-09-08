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
#include "../Helpers/ImageManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

CategoryScene::~CategoryScene()
{
}

CategoryScene::CategoryScene()
{
    m_pBolasSprites = new Vector<Sprite*>();
}

bool CategoryScene::init()
{
    kategoriak = SQLHelper::getInstance()->queryKategoriak();
    
    int pages = (int) kategoriak->size() / 4;
    
    if (kategoriak->size() / 4 != 0) {
        pages++;
    }
    
    Size size = Director::getInstance()->getWinSize();
    
    Layer *pBglayer = Layer::create();
    pBglayer->setContentSize(size);
    
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    this->addChild(fondo);
    
    Sprite *leftGrass = Sprite::create("grass_left.png");
    leftGrass->setAnchorPoint(Vec2(0,0));
    leftGrass->setPosition(VisibleRect::leftBottom());
    addChild(leftGrass);
    Sprite *rightGrass = Sprite::create("grass_right.png");
    rightGrass->setAnchorPoint(Vec2(1,0));
    rightGrass->setPosition(VisibleRect::rightBottom());
    addChild(rightGrass);
    
    Sprite *pTitle = Sprite::create("gaiak.png");
    pTitle->setPosition(Vec2(VisibleRect::center().x, VisibleRect::top().y - 90));
    pBglayer->addChild(pTitle);
    
    auto pBackButton = SpriteButton::create(ImageManager::getImage("go-back"), 1.0f, CC_CALLBACK_1(CategoryScene::backButton, this));
    pBackButton->setAnchorPoint(Vec2(0, 1));
    pBackButton->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y));
    pBglayer->addChild(pBackButton);
    
    auto pScrollLayers = new Vector<Layer*>(10);
    Size scrollLayerSize = Size(size.width, pTitle->getPosition().y - pTitle->getContentSize().height/2 - 10);
    Size buttonSize = Size(290, 54);
    
    
    //Calculate padding
    float tmp = scrollLayerSize.height - (4.0 * buttonSize.height) - VisibleRect::bottom().y + 25;
    float padding = (tmp / 8);
    
    int numberRows = (int) kategoriak->size();
            
    for (int i = 0; i <= pages; i++) {
        
        Point initialPosition = Vec2(pTitle->getPosition().x, pTitle->getPosition().y - pTitle->getContentSize().height - 10);
        
        //LayerColor *pageLayer = LayerColor::create(Color4B(255, 0, 0, 255));
        Layer *pageLayer = Layer::create();
        pageLayer->setPosition(Vec2(size.width/2, size.height/2));
        pageLayer->setContentSize(scrollLayerSize);
        
        if (numberRows == 0) {
            goto exit;
        }
            
        for (int j = 0; j < 4; j++) {
            
            numberRows--;
            
            int position = i*4+j;
            KategoriaModel *kategoria = kategoriak->at(position);
            
            //15 caracteres
            auto label = Label::createWithTTF(kategoria->izena.c_str(), "fonts/PT_Sans-Web-Regular.ttf", 15.0);
            //TODO
            auto pButton = SpriteButton::create(ImageManager::getImage("botoia_normal"), 0.9f, CC_CALLBACK_1(CategoryScene::loadLevels, this));
            
            pButton->setTag(position);
                        
            Point buttonPoint = pageLayer->getPosition();
            pButton->setPosition(initialPosition);
            label->setPosition(Vec2(initialPosition.x, initialPosition.y));
            pageLayer->addChild(pButton);
            pageLayer->addChild(label);
            
            if (numberRows == 0) {
                break;
            }
            
            initialPosition.y -= pButton->getContentSize().height + padding;
        }
        
        pScrollLayers->pushBack(pageLayer);
        
        if (numberRows == 0) {
            goto exit;
        }
    }
    
exit:

    Size scollFrameSize = Director::getInstance()->getWinSize();
    auto pScrollLayer = ScrollLayer::nodeWithLayers(pScrollLayers, 0);
    pScrollLayer->setPagesIndicatorPosition(Vec2(VisibleRect::center().x, VisibleRect::bottom().y + 10));
    pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(0);
    pBglayer->addChild(pScrollLayer);
    
    addChild(pBglayer);
    
    return true;
}

void CategoryScene::scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page)
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

void CategoryScene::backButton(Ref* sender)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    MainMenuScene *scene = MainMenuScene::create();
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void CategoryScene::loadLevels(Ref* sender)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    ControlButton *button = (ControlButton*) sender;
    KategoriaModel *kategoria = (KategoriaModel*) kategoriak->at(button->getTag());
    
    LevelScene *scene = LevelScene::create(kategoria);
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

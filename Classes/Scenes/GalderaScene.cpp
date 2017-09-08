//
//  GalderaScene.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 08/01/13.
//
//

#include "../Scenes/GalderaScene.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/SpriteButton.h"
#include "../Scenes/CategoryScene.h"
#include "../Scenes/BukatutaScene.h"
#include "SimpleAudioEngine.h"
#include "../Helpers/Globals.h"
#include "../Helpers/ImageManager.h"

using namespace CocosDenshion;

GalderaScene::~GalderaScene()
{
    
}

GalderaScene::GalderaScene()
{
    
}


// on "init" you need to initialize your instance
bool GalderaScene::init()
{    
    visualRect = VisibleRect::getVisibleRect();
    
    LayerGradient *pBglayer = LayerGradient::create(Color4B(154, 182, 0, 255), Color4B(133, 160, 0, 255));
    
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    
    pBglayer->addChild(fondo);
    
    galderaLayer = Layer::create();
    
    galderaNumber = Label::createWithTTF("20", "fonts/CarterOne.ttf", 25.0);
    galderaNumber->setColor(Color3B(0, 0, 0));
    galderaNumber->setPosition(Vec2( VisibleRect::top().x,  VisibleRect::top().y - 30));
    galderaLayer->addChild(galderaNumber);
    
    auto pBackButton = SpriteButton::create(ImageManager::getImage("go-back"), 1.0f, CC_CALLBACK_1(GalderaScene::backButton, this));
    pBackButton->setAnchorPoint(Vec2(0, 1));
    pBackButton->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y));
    pBglayer->addChild(pBackButton);
    
    galdera = Label::createWithTTF("Esto hay que poner para que cocos calcule el tamaño de la pregunta. Nunca sale. Trulalalala jamon", "fonts/CarterOne.ttf", 18.0);
   
    galdera->setColor(Color3B(0, 0, 0));
    galdera->setAnchorPoint(Vec2(0.5, 1));
    galdera->setPosition(Vec2(VisibleRect::center().x, galderaNumber->getPosition().y - 30));
    galdera->setDimensions(VisibleRect::getVisibleRect().size.width - 20, 70);
    galdera->setVerticalAlignment(TextVAlignment::CENTER);
    galdera->setHorizontalAlignment(TextHAlignment::CENTER);
    galderaLayer->addChild(galdera);
    
    
    Point point = Vec2(VisibleRect::center().x, galdera->getPosition().y - galdera->getContentSize().height - 40);
    
       
    float height = visualRect.size.height - (galdera->getPosition().y + galdera->getContentSize().height/2);
    float padding = height / 2;
    
    
    Label *pLabel1 = Label::createWithTTF("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    ui::Scale9Sprite *sprite1 = ui::Scale9Sprite::create("botoia_normal.png");
    pErantzuna1 = ControlButton::create(pLabel1, sprite1);
    pErantzuna1->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    pErantzuna1->setPreferredSize(Size(213, 56));
    pErantzuna1->setPosition(Vec2(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna1);
    
    Size erantzunaSize = Size(pErantzuna1->getContentSize().width, pErantzuna1->getContentSize().height);
    
    point.y-=padding + erantzunaSize.height;
    
    Label *pLabel2 = Label::createWithTTF("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    ui::Scale9Sprite *sprite2 = ui::Scale9Sprite::create("botoia_normal.png");
    pErantzuna2 = ControlButton::create(pLabel2, sprite2);
    pErantzuna2->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    pErantzuna2->setPreferredSize(Size(213, 56));
    pErantzuna2->setPosition(Vec2(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna2);
    
    point.y-=padding + erantzunaSize.height;
    
    Label *pLabel3 = Label::createWithTTF("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    ui::Scale9Sprite *sprite3 = ui::Scale9Sprite::create("botoia_normal.png");
    pErantzuna3 = ControlButton::create(pLabel3, sprite3);
    pErantzuna3->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    pErantzuna3->setPreferredSize(Size(213, 56));
    pErantzuna3->setPosition(Vec2(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna3);
    
    point.y-=padding + erantzunaSize.height;
    
    Label *pLabel4 = Label::createWithTTF("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    ui::Scale9Sprite *sprite4 = ui::Scale9Sprite::create("botoia_normal.png");
    pErantzuna4 = ControlButton::create(pLabel4, sprite4);
    pErantzuna4->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), cocos2d
                                                     ::extension::Control::EventType::TOUCH_UP_INSIDE);
    pErantzuna4->setPreferredSize(Size(213, 56));
    pErantzuna4->setPosition(Vec2(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna4);
    
    point.y-=erantzunaSize.height;
    
    pBarra = Sprite::create("progress_bar_05.png");
    pBarra->setPosition(Vec2(point.x, VisibleRect::bottom().y + 30));
    galderaLayer->addChild(pBarra);
    
    pBglayer->addChild(galderaLayer);
    
    addChild(pBglayer);
    
    pSQL = SQLHelper::getInstance();
    
    pGalderak = pSQL->queryGalderak(multzoa->idMultzoa);
    
    galderaCount = (int) pGalderak->size();
    currentGalderaPosition = multzoa->currentGaldera;
    kalifikazioa = multzoa->kalifikazioa;
    
    newGaldera(NULL);

	return true;
}

void GalderaScene::newGaldera(Node* sender)
{
    if (sender == NULL) {
        currentGaldera = (GalderaModel*) pGalderak->at(currentGalderaPosition - 1);
    } else {
        currentGaldera = (GalderaModel*) pGalderak->at(currentGalderaPosition - 1);
    }
    
    char galderaZenb[3];
    sprintf(galderaZenb, "%i", currentGalderaPosition);
    galderaNumber->setString(galderaZenb);
    
    galdera->setString(currentGaldera->galdera.c_str());
        
    int correctPosition = rand() % 3 + 1;
    //int correctPosition = rand() % 2 + 1;
    
    switch (correctPosition) {
        case 0:
            pErantzuna1->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::NORMAL);
            pErantzuna1->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna1->setTag(-1);
            
            pErantzuna2->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::NORMAL);
            pErantzuna2->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna2->setTag(0);
            
            pErantzuna3->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::NORMAL);
            pErantzuna3->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna3->setTag(0);
            
            pErantzuna4->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::NORMAL);
            pErantzuna4->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna4->setTag(0);
            
            break;
            
        case 1:
            pErantzuna1->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::NORMAL);
            pErantzuna1->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna1->setTag(0);
            
            pErantzuna2->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::NORMAL);
            pErantzuna2->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna2->setTag(-1);
            
            pErantzuna3->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::NORMAL);
            pErantzuna3->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna3->setTag(0);
            
            pErantzuna4->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::NORMAL);
            pErantzuna4->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna4->setTag(0);
            
            break;
            
        case 2:
            pErantzuna1->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::NORMAL);
            pErantzuna1->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna1->setTag(0);
            
            pErantzuna2->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::NORMAL);
            pErantzuna2->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna2->setTag(0);
            
            pErantzuna3->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::NORMAL);
            pErantzuna3->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna3->setTag(-1);
            
            pErantzuna4->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::NORMAL);
            pErantzuna4->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna4->setTag(0);
            
            break;
            
        case 3:
            pErantzuna1->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::NORMAL);
            pErantzuna1->setTitleForState(currentGaldera->erantzuna3.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna1->setTag(0);
            
            pErantzuna2->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::NORMAL);
            pErantzuna2->setTitleForState(currentGaldera->erantzuna1.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna2->setTag(0);
            
            pErantzuna3->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::NORMAL);
            pErantzuna3->setTitleForState(currentGaldera->erantzuna2.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna3->setTag(0);
            
            pErantzuna4->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::NORMAL);
            pErantzuna4->setTitleForState(currentGaldera->zuzena.c_str(), Control::State::HIGH_LIGHTED);
            pErantzuna4->setTag(-1);
            
            break;
            
        default:
            break;
    }
    
    Texture2D* t = Director::getInstance()->getTextureCache()->addImage("progress_bar_05.png");
    pBarra->setTexture(t);

    barraCount = 5;
    
    this->schedule(schedule_selector(GalderaScene::gameLogic), kQuestionSeconds );
}


void GalderaScene::answerClicked(Ref *pSender, cocos2d::extension::Control::EventType pCCControlEvent)
{
    this->unschedule(schedule_selector(GalderaScene::gameLogic));
    
    childrenTouchEnabled(false);
    
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    
    Sprite *answerSprite = checkAnswer(pSender);
    
    FiniteTimeAction *actionDelay = DelayTime::create(2.0);
    FiniteTimeAction *actionShow = ScaleTo::create(0.2, 1);
    FiniteTimeAction* actionScaleDone = CallFuncN::create(CC_CALLBACK_1(GalderaScene::exitGaldera, this));
    answerSprite->runAction(Sequence::create(actionShow, actionDelay, actionScaleDone, NULL));
}

Sprite* GalderaScene::checkAnswer(Ref *pSender)
{
    Sprite *answerSprite;
    std::string sound;
    float bonus = 1;
    int tag = ((ControlButton*)pSender)->getTag();
    if (tag == -1) {
        answerSprite = Sprite::create("popup_galdera_ondo.png");
        if (barraCount < 0)
            barraCount = 1;
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
        kalifikazioa = kalifikazioa + ((kQuestionPoints * bonus) * barraCount);
        sound.append("sounds/bien.wav");
    } else {
        sound.append("sounds/mal.wav");
        answerSprite = Sprite::create("popup_galdera_oker.png");
    }
    
    log("QUESTIONPOINTS: %f", (kQuestionPoints * bonus));
    log("PUNTUAZIOA: %d", kalifikazioa);
    log("BARRACOUNT: %d", barraCount);
    
    answerSprite->setPosition(VisibleRect::center());
    answerSprite->setScale(0.0);
    answerSprite->setTag(1);
    galderaLayer->addChild(answerSprite);
    
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename(sound.c_str())).c_str());
    
    log("Kalifikazioa = %d, galderaCount = %d", kalifikazioa, galderaCount);
    
    return answerSprite;
}

void GalderaScene::exitGaldera(Node* sender)
{
    FiniteTimeAction *actionMove = MoveBy::create(0.2, Size(VisibleRect::getVisibleRect().size.width, 0));
    FiniteTimeAction* actionMoveDone = CallFuncN::create(CC_CALLBACK_1(GalderaScene::changeGaldera, this));
    FiniteTimeAction *actionDelay = DelayTime::create(2.0);
    galderaLayer->runAction(Sequence::create(actionMove, actionMoveDone, actionDelay, NULL));
}

void GalderaScene::changeGaldera(Node* sender)
{
    galderaLayer->removeChildByTag(1, true);
    
    childrenTouchEnabled(true);
    
    newGaldera(sender);
    
    currentGalderaPosition++;
    
    if (currentGalderaPosition == (galderaCount + 1)) {
        finish();
    } else {
        
        galderaLayer->setPosition(Size(-visualRect.size.width, 0));
        FiniteTimeAction *actionMove = MoveBy::create(0.2, Size(visualRect.size.width, 0));
        FiniteTimeAction* actionMoveDone = CallFuncN::create(CC_CALLBACK_1(GalderaScene::newGaldera, this));
        galderaLayer->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
    }
}

void GalderaScene::gameLogic(float dt)
{
    Texture2D* t;
    switch (barraCount) {
        case 5:
            t = Director::getInstance()->getTextureCache()->addImage("progress_bar_04.png");
            break;
        case 4:
            t = Director::getInstance()->getTextureCache()->addImage("progress_bar_03.png");
            break;
        case 3:
            t = Director::getInstance()->getTextureCache()->addImage("progress_bar_02.png");
            break;
        case 2:
            t = Director::getInstance()->getTextureCache()->addImage("progress_bar_01.png");
            break;
        case 1:
            t = Director::getInstance()->getTextureCache()->addImage("progress_bar_00.png");
            break;
        default:
            t = Director::getInstance()->getTextureCache()->addImage("progress_bar_00.png");
            break;
    }
    pBarra->setTexture(t);
    barraCount--;
}

void GalderaScene::finish()
{
    multzoa->kalifikazioa = kalifikazioa;
    multzoa->currentGaldera = currentGalderaPosition;
    multzoa->bukatuta = 1;
    
    pSQL->updateErabiltzaileMultzoa(multzoa);
    
    BukatutaScene *scene = BukatutaScene::create(multzoa);
    
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void GalderaScene::childrenTouchEnabled(bool enabled)
{
    
    for (auto item: galderaLayer->getChildren())
    {
        Node *children = (Node*) item;
        Layer* v = dynamic_cast<Layer*>(children);
            if(v != 0)
                v->setTouchEnabled(enabled);
    }
}


void GalderaScene::backButton(Ref* sender)
{
    multzoa->kalifikazioa = kalifikazioa;
    multzoa->currentGaldera = currentGalderaPosition;
    multzoa->bukatuta = 0;
    
    pSQL->updateErabiltzaileMultzoa(multzoa);
    
    CategoryScene *scene = CategoryScene::create();
    
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}


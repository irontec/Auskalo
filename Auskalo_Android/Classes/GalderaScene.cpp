//
//  GalderaScene.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 08/01/13.
//
//

#include "GalderaScene.h"
#include "VisibleRect.h"
#include "SpriteButton.h"
#include "CategoryScene.h"
#include "BukatutaScene.h"
#include "SimpleAudioEngine.h"
#include "CCNodeLoaderLibrary.h"
#include "../Helpers/Globals.h"

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
    
    CCLayerGradient *pBglayer = CCLayerGradient::create(ccc4(154, 182, 0, 255), ccc4(133, 160, 0, 255));
    galderaLayer = CCLayer::create();
    
    galderaNumber = CCLabelTTF::create("20", "font/CarterOne.ttf", 25.0);
    galderaNumber->setColor(ccc3(0, 0, 0));
    galderaNumber->setPosition(ccp( VisibleRect::top().x,  VisibleRect::top().y - 30));
    galderaNumber->setFontName("fonts/CarterOne.ttf");
    galderaLayer->addChild(galderaNumber);
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("go-back.png");
    SpriteButton *pBackButton = SpriteButton::create(t, this, callfuncO_selector(GalderaScene::backButton));
    pBackButton->setAnchorPoint(ccp(0, 1));
    pBackButton->setPosition(ccp(VisibleRect::leftTop().x, VisibleRect::leftTop().y));
    pBglayer->addChild(pBackButton);
    
    galdera = CCLabelTTF::create("Esto hay que poner para que cocos calcule el tamaño de la pregunta. Nunca sale. Trulalalala jamon", "font/CarterOne.ttf", 18.0);
   
    galdera->setColor(ccc3(0, 0, 0));
    galdera->setAnchorPoint(ccp(0.5, 1));
    galdera->setPosition(ccp(VisibleRect::center().x, galderaNumber->getPosition().y - 30));
    galdera->setFontName("fonts/CarterOne.ttf");
    galdera->setDimensions(CCSizeMake(VisibleRect::getVisibleRect().size.width - 20, 70));
    galdera->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    galdera->setHorizontalAlignment(kCCTextAlignmentCenter);
    galderaLayer->addChild(galdera);
    
    
    CCPoint point = ccp(VisibleRect::center().x, galdera->getPosition().y - galdera->getContentSize().height - 40);
    
       
    float height = visualRect.size.height - (galdera->getPosition().y + galdera->getContentSize().height/2);
    float padding = height / 2;
    
    
    CCLabelTTF *pLabel1 = CCLabelTTF::create("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    CCScale9Sprite *sprite1 = CCScale9Sprite::create("botoia_normal.png");
    pErantzuna1 = CCControlButton::create(pLabel1, sprite1);
    pErantzuna1->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), CCControlEventTouchUpInside);
    pErantzuna1->setPreferredSize(CCSizeMake(213, 56));
    pErantzuna1->setPosition(ccp(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna1);
    
    CCSize erantzunaSize = CCSizeMake(pErantzuna1->getContentSize().width, pErantzuna1->getContentSize().height);
    
    point.y-=padding + erantzunaSize.height;
    
    CCLabelTTF *pLabel2 = CCLabelTTF::create("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    CCScale9Sprite *sprite2 = CCScale9Sprite::create("botoia_normal.png");
    pErantzuna2 = CCControlButton::create(pLabel2, sprite2);
    pErantzuna2->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), CCControlEventTouchUpInside);
    pErantzuna2->setPreferredSize(CCSizeMake(213, 56));
    pErantzuna2->setPosition(ccp(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna2);
    
    point.y-=padding + erantzunaSize.height;
    
    CCLabelTTF *pLabel3 = CCLabelTTF::create("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    CCScale9Sprite *sprite3 = CCScale9Sprite::create("botoia_normal.png");
    pErantzuna3 = CCControlButton::create(pLabel3, sprite3);
    pErantzuna3->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), CCControlEventTouchUpInside);
    pErantzuna3->setPreferredSize(CCSizeMake(213, 56));
    pErantzuna3->setPosition(ccp(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna3);
    
    point.y-=padding + erantzunaSize.height;
    
    CCLabelTTF *pLabel4 = CCLabelTTF::create("", "fonts/PT_Sans-Web-Regular.ttf", 15.0);
    CCScale9Sprite *sprite4 = CCScale9Sprite::create("botoia_normal.png");
    pErantzuna4 = CCControlButton::create(pLabel4, sprite4);
    pErantzuna4->addTargetWithActionForControlEvents(this, cccontrol_selector(GalderaScene::answerClicked), CCControlEventTouchUpInside);
    pErantzuna4->setPreferredSize(CCSizeMake(213, 56));
    pErantzuna4->setPosition(ccp(point.x, point.y));
    
    galderaLayer->addChild(pErantzuna4);
    
    point.y-=erantzunaSize.height;
    
    pBarra = CCSprite::create("progress_bar_05.png");
    pBarra->setPosition(ccp(point.x, VisibleRect::bottom().y + 30));
    galderaLayer->addChild(pBarra);
    
    pBglayer->addChild(galderaLayer);
    
    addChild(pBglayer);
    
    pSQL = SQLHelper::sharedInstance();
    
    pGalderak = pSQL->queryGalderak(multzoa->idMultzoa);
    pGalderak->retain();
    
    galderaCount = pGalderak->count();
    currentGalderaPosition = multzoa->currentGaldera;
    kalifikazioa = multzoa->kalifikazioa;
    
    newGaldera(NULL);

	return true;
}

void GalderaScene::newGaldera(CCNode* sender)
{
    if (sender == NULL) {
        currentGaldera = (GalderaModel*) pGalderak->objectAtIndex(currentGalderaPosition - 1);
    } else {
        currentGaldera = (GalderaModel*) pGalderak->objectAtIndex(currentGalderaPosition - 1);
    }
    
    char galderaZenb[3];
    sprintf(galderaZenb, "%i", currentGalderaPosition);
    galderaNumber->setString(galderaZenb);
    
    galdera->setString(currentGaldera->galdera.c_str());
        
    int correctPosition = rand() % 3 + 1;
    
    switch (correctPosition) {
        case 0:
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateNormal);
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateHighlighted);
            pErantzuna1->setTag(-1);
            
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateNormal);
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateHighlighted);
            pErantzuna2->setTag(0);
            
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateNormal);
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateHighlighted);
            pErantzuna3->setTag(0);
            
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->erantzuna3.c_str()), CCControlStateNormal);
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->erantzuna3.c_str()), CCControlStateHighlighted);
            pErantzuna4->setTag(0);
            
            break;
            
        case 1:
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateNormal);
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateHighlighted);
            pErantzuna1->setTag(0);
            
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateNormal);
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateHighlighted);
            pErantzuna2->setTag(-1);
            
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateNormal);
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateHighlighted);
            pErantzuna3->setTag(0);
            
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->erantzuna3.c_str()), CCControlStateNormal);
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->erantzuna3.c_str()), CCControlStateHighlighted);
            pErantzuna4->setTag(0);
            
            break;
            
        case 2:
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateNormal);
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateHighlighted);
            pErantzuna1->setTag(0);
            
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateNormal);
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateHighlighted);
            pErantzuna2->setTag(0);
            
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateNormal);
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateHighlighted);
            pErantzuna3->setTag(-1);
            
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->erantzuna3.c_str()), CCControlStateNormal);
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->erantzuna3.c_str()), CCControlStateHighlighted);
            pErantzuna4->setTag(0);
            
            break;
            
        case 3:
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->erantzuna3.c_str()), CCControlStateNormal);
            pErantzuna1->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateHighlighted);
            pErantzuna1->setTag(0);
            
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateNormal);
            pErantzuna2->setTitleForState(CCString::create(currentGaldera->erantzuna1.c_str()), CCControlStateHighlighted);
            pErantzuna2->setTag(0);
            
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateNormal);
            pErantzuna3->setTitleForState(CCString::create(currentGaldera->erantzuna2.c_str()), CCControlStateHighlighted);
            pErantzuna3->setTag(0);
            
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateNormal);
            pErantzuna4->setTitleForState(CCString::create(currentGaldera->zuzena.c_str()), CCControlStateHighlighted);
            pErantzuna4->setTag(-1);
            
            break;
            
        default:
            break;
    }
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("progress_bar_05.png");
    pBarra->setTexture(t);

    barraCount = 5;
    
    this->schedule(schedule_selector(GalderaScene::gameLogic), kQuestionSeconds );
}


void GalderaScene::answerClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    this->unschedule(schedule_selector(GalderaScene::gameLogic));
    
    childrenTouchEnabled(false);
    
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    
    CCSprite *answerSprite = checkAnswer(pSender);
    
    CCFiniteTimeAction *actionDelay = CCDelayTime::create(2.0);
    CCFiniteTimeAction *actionShow = CCScaleTo::create(0.2, 1);
    CCFiniteTimeAction* actionScaleDone = CCCallFuncN::create(this, callfuncN_selector(GalderaScene::exitGaldera));
    answerSprite->runAction(CCSequence::create(actionShow, actionDelay, actionScaleDone, NULL));
}

CCSprite* GalderaScene::checkAnswer(CCObject *pSender)
{
    CCSprite *answerSprite;
    std::string sound;
    
    int tag = ((CCControlButton*)pSender)->getTag();
    
    if (tag == -1) {
        answerSprite = CCSprite::create("popup_galdera_ondo.png");
        if (barraCount < 0)
            barraCount = 1;
        kalifikazioa = kalifikazioa + (kQuestionPoints * barraCount);
        sound.append("sounds/bien.wav");
    } else {
        sound.append("sounds/mal.wav");
        answerSprite = CCSprite::create("popup_galdera_oker.png");
    }
    
    answerSprite->setPosition(VisibleRect::center());
    answerSprite->setScale(0.0);
    answerSprite->setTag(1);
    galderaLayer->addChild(answerSprite);
    
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(sound.c_str())).c_str());
    
    return answerSprite;
}

void GalderaScene::exitGaldera(CCNode* sender)
{
    CCFiniteTimeAction *actionMove = CCMoveBy::create(0.2, CCPointMake(VisibleRect::getVisibleRect().size.width, 0));
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GalderaScene::changeGaldera));
    CCFiniteTimeAction *actionDelay = CCDelayTime::create(2.0);
    galderaLayer->runAction(CCSequence::create(actionMove, actionMoveDone, actionDelay, NULL));
}

void GalderaScene::changeGaldera(CCNode* sender)
{
    galderaLayer->removeChildByTag(1, true);
    
    childrenTouchEnabled(true);
    
    newGaldera(sender);
    
    currentGalderaPosition++;
    
    if (currentGalderaPosition == (galderaCount + 1)) {
        finish();
    } else {
        
        galderaLayer->setPosition(CCPointMake(-visualRect.size.width, 0));
        CCFiniteTimeAction *actionMove = CCMoveBy::create(0.2, CCPointMake(visualRect.size.width, 0));
        CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GalderaScene::newGaldera));
        galderaLayer->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
    }
}

void GalderaScene::gameLogic(float dt)
{
    CCTexture2D* t;
    switch (barraCount) {
        case 5:
            t = CCTextureCache::sharedTextureCache()->addImage("progress_bar_04.png");
            break;
        case 4:
            t = CCTextureCache::sharedTextureCache()->addImage("progress_bar_03.png");
            break;
        case 3:
            t = CCTextureCache::sharedTextureCache()->addImage("progress_bar_02.png");
            break;
        case 2:
            t = CCTextureCache::sharedTextureCache()->addImage("progress_bar_01.png");
            break;
        case 1:
            t = CCTextureCache::sharedTextureCache()->addImage("progress_bar_00.png");
            break;
        default:
            t = CCTextureCache::sharedTextureCache()->addImage("progress_bar_00.png");
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
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void GalderaScene::childrenTouchEnabled(bool enabled)
{
    CCObject *item;
    
    CCARRAY_FOREACH(galderaLayer->getChildren(), item)
    {
        CCNode *children = (CCNode*) item;
        CCLayer* v = dynamic_cast<CCLayer*>(children);
        if(v != 0)
            v->setTouchEnabled(enabled);
    }
}


void GalderaScene::backButton(CCObject *sender, CCControlEvent controlEvent)
{
    multzoa->kalifikazioa = kalifikazioa;
    multzoa->currentGaldera = currentGalderaPosition;
    multzoa->bukatuta = 0;
    
    pSQL->updateErabiltzaileMultzoa(multzoa);
    
    CategoryScene *scene = CategoryScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}


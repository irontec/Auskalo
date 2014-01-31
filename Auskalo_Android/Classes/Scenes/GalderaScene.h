//
//  GalderaScene.h
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 08/01/13.
//
//

#ifndef __TrivialJokoa__GalderaScene__
#define __TrivialJokoa__GalderaScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Models/MultzoaModel.h"
#include "../Models/GalderaModel.h"
#include "../Helpers/SQLHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GalderaScene : public CCScene
{
public:
    GalderaScene();
    ~GalderaScene();
    virtual bool init();
    
    static CCScene* scene();
    
    static GalderaScene* create(MultzoaModel *multzoa)
    {
        GalderaScene *pRet = new GalderaScene();
        pRet->multzoa = multzoa;
        
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }

private:
    
    CCRect visualRect;
    
    MultzoaModel *multzoa;
    SQLHelper *pSQL;
    CCArray *pGalderak;
    GalderaModel *currentGaldera;
    
    int currentGalderaPosition;
    int galderaCount;
    int galderaGoodCount;
    int barraCount;
    int kalifikazioa;
    
    CCLayer *galderaLayer;
    CCLabelTTF *galdera;
    CCLabelTTF *galderaNumber;
    CCControlButton *pErantzuna1, *pErantzuna2, *pErantzuna3, *pErantzuna4;
    CCSprite *pBarra;
    
    void childrenTouchEnabled(bool enabled);
    void changeGaldera(CCNode* sender);
    void newGaldera(CCNode* sender);
    void exitGaldera(CCNode* sender);
    void answerClicked(CCObject *pSender, CCControlEvent pCCControlEvent);
    void finish();
    void backButton(CCObject *sender, CCControlEvent controlEvent);
    void gameLogic(float dt);
    CCSprite* checkAnswer(CCObject *pSender);
};


#endif /* defined(__TrivialJokoa__GalderaScene__) */

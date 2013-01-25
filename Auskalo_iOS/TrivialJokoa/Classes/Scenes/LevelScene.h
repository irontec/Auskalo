//
//  LevelScene.h
//  TrivialJokoa
//
//  Created by iker on 13/12/12.
//
//

#ifndef __TrivialJokoa__LevelSelectorScene__
#define __TrivialJokoa__LevelSelectorScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KategoriaModel.h"
#include "SpriteButton.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MultzoaModel;

class LevelScene : public CCScene
{
public:
    LevelScene();
	~LevelScene();
	virtual bool init();
    
    static LevelScene* create(KategoriaModel kategoria)
    {
        LevelScene *pRet = new LevelScene();
        pRet->kategoria = kategoria;
        
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
    KategoriaModel kategoria;
    CCArray *m_pButtonsArray;
    CCArray *multzoak;
    
    void backButton(CCObject *sender, CCControlEvent controlEvent);
    void levelSelected(CCObject *o);
    SpriteButton* getButton(MultzoaModel *multzoa);
};

#endif /* defined(__TrivialJokoa__LevelScene__) */

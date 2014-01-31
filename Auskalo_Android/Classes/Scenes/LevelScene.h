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
#include "../Models/KategoriaModel.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/CCScrollLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MultzoaModel;

class LevelScene : public CCScene, public CCScrollLayerDelegate
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
    
    CCArray *m_pBolasSprites;
    
    void backButton(CCObject *sender, CCControlEvent controlEvent);
    void levelSelected(CCObject *o);
    SpriteButton* getButton(MultzoaModel *multzoa);
    
    void scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page);
};

#endif /* defined(__TrivialJokoa__LevelScene__) */

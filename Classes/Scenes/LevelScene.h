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
#include "../Helpers/ScrollLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MultzoaModel;

class LevelScene : public Scene, public ScrollLayerDelegate
{
public:
    LevelScene();
	~LevelScene();
	virtual bool init();
    
    static LevelScene* create(KategoriaModel *kategoria)
    {
        LevelScene *pRet = new LevelScene();
        pRet->kategoria = kategoria;
        pRet->init();
        return pRet;
    }
private:
    KategoriaModel *kategoria;
    Vector<Ref*> * m_pButtonsArray;
    Vector<Ref*> * multzoak;

    ScrollLayer *pScrollLayer;
    
    Vector<Sprite*> * m_pBolasSprites;
    
    void backButton(Ref* send);
    void levelSelected(Ref *o);
    SpriteButton* getButton(MultzoaModel *multzoa);
    
    void scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page);
};

#endif /* defined(__TrivialJokoa__LevelScene__) */

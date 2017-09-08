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

class GalderaScene : public Scene
{
public:
    GalderaScene();
    ~GalderaScene();
    virtual bool init();
    
    static Scene* scene();
    
    static GalderaScene* create(MultzoaModel *multzoa)
    {
        GalderaScene *pRet = new GalderaScene();
        pRet->multzoa = multzoa;
        pRet->init();
        return pRet;
    }

private:
    
    Rect visualRect;
    
    MultzoaModel *multzoa;
    SQLHelper *pSQL;
    Vector<Ref*> * pGalderak;
    GalderaModel *currentGaldera;
    
    int currentGalderaPosition;
    int galderaCount;
    int galderaGoodCount;
    int barraCount;
    int kalifikazioa;
    
    Layer *galderaLayer;
    Label *galdera;
    Label *galderaNumber;
    ControlButton *pErantzuna1, *pErantzuna2, *pErantzuna3, *pErantzuna4;
    Sprite *pBarra;
    
    void childrenTouchEnabled(bool enabled);
    void changeGaldera(Node* sender);
    void newGaldera(Node* sender);
    void exitGaldera(Node* sender);
    void answerClicked(Ref *pSender, cocos2d::extension::Control::EventType pCCControlEvent);
    void finish();
    void backButton(Ref* sender);
    void gameLogic(float dt);
    Sprite* checkAnswer(Ref *pSender);
};


#endif /* defined(__TrivialJokoa__GalderaScene__) */

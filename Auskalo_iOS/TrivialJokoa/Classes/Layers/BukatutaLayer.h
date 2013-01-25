//
//  BukatutaLayer.h
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 11/01/13.
//
//

#ifndef __TrivialJokoa__BukatutaLayer__
#define __TrivialJokoa__BukatutaLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MultzoaModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

class BukatutaLayer : public CCLayer, public CCBSelectorResolver, public CCNodeLoaderListener
{
public:
    //CREATE_FUNC(BukatutaLayer);
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BukatutaLayer, create);
    
    BukatutaLayer();
    virtual ~BukatutaLayer();
    virtual bool init();
    virtual void onEnter();
    
    MultzoaModel *multzoa;
    
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
    
private:
    CCSprite *pImage;
    CCSprite *pStars;
    
    
    void buttonClicked(CCObject *pSender, CCControlEvent pCCControlEvent);
};

class BukatutaLayerLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(BukatutaLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BukatutaLayer);
};



#endif /* defined(__TrivialJokoa__BukatutaLayer__) */

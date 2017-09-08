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
#include "../Models/MultzoaModel.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocosbuilder;

class BukatutaLayer : public Layer, public CCBSelectorResolver, public NodeLoaderListener
{
public:
    //CREATE_FUNC(BukatutaLayer);
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BukatutaLayer, create);
    
    BukatutaLayer();
    virtual ~BukatutaLayer();
    virtual bool init();
    virtual void onEnter();
    
    MultzoaModel *multzoa;
    
    virtual void onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName);
    
private:
    Sprite *pImage;
    Sprite *pStars;
    void buttonClicked(Ref *pSender, cocos2d::extension::Control::EventType pCCControlEvent);
};

class BukatutaLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(BukatutaLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BukatutaLayer);
};



#endif /* defined(__TrivialJokoa__BukatutaLayer__) */

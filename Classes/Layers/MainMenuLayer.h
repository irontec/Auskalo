//
//  MainMenuLayer.h
//  TrivialJokoa
//
//  Created by iker on 11/12/12.
//
//

#ifndef __TrivialJokoa__MainMenuLayer__
#define __TrivialJokoa__MainMenuLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MainMenuLayer : public Layer, public cocosbuilder::CCBSelectorResolver, cocosbuilder::NodeLoaderListener
{
public:
    cocosbuilder::CCBAnimationManager* animationManager;
public:
    CREATE_FUNC(MainMenuLayer);
    //CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainMenuLayer, create);
    
    MainMenuLayer();
    virtual ~MainMenuLayer();
    bool init();

    //void onNodeLoaded(Node * pNode, CCNodeLoader * pNodeLoader);
    //virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, CCString * pSelectorName);
    //virtual SEL_CCControlHandler onResolveCCBCCControlSelector(Ref * pTarget, CCString * pSelectorName);
    
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    
private:
    void playClicked(Ref *pSender, cocos2d::extension::Control::EventType pCCControlEvent);
    void profilaClicked(Ref *pSender, cocos2d::extension::Control::EventType pCCControlEvent);
    void buruzClicked(Ref *pSender, cocos2d::extension::Control::EventType pCCControlEvent);
};

class MainMenuLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainMenuLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MainMenuLayer);
};

#endif /* defined(__TrivialJokoa__MainMenuLayer__) */

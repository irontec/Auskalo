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
#include "CCBAnimationManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MainMenuLayer : public CCLayer, public CCBSelectorResolver, CCNodeLoaderListener
{
public:
    CCBAnimationManager* animationManager;
public:
    CREATE_FUNC(MainMenuLayer);
    //CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainMenuLayer, create);
    
    MainMenuLayer();
    virtual ~MainMenuLayer();
    bool init();

    //void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
    //virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    //virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
    
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
private:
    void playClicked(CCObject *pSender, CCControlEvent pCCControlEvent);
    void profilaClicked(CCObject *pSender, CCControlEvent pCCControlEvent);
    void buruzClicked(CCObject *pSender, CCControlEvent pCCControlEvent);
};

class MainMenuLayerLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainMenuLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MainMenuLayer);
};

#endif /* defined(__TrivialJokoa__MainMenuLayer__) */

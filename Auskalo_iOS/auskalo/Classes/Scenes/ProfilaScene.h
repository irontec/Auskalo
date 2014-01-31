//
//  ProfilaScene.h
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 14/01/13.
//
//

#ifndef __TrivialJokoa__ProfilaScene__
#define __TrivialJokoa__ProfilaScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Helpers/SQLHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ProfilaScene : public CCScene
{
public:
    ProfilaScene();
	~ProfilaScene();
	virtual bool init();
    
	// implement the "static node()" method manually
	CREATE_FUNC(ProfilaScene);
private:
    SQLHelper *pSQL;
    CCLayer *pSailkapenaLayer;
    CCLayerColor *layer;
    
    CCLabelTTF *pAbisua;
    
    void backButton(CCObject *sender, CCControlEvent controlEvent);
    void loginCallback(CCObject *pSender);
    void reset(CCObject *sender, CCControlEvent controlEvent);
    void onHttpRequestCompleted(CCNode *sender, void *data);
};

class ProfilaLayerLoader : public CCLayerLoader
{
public:
//    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ProfilaLayerLoader, loader);
};



#endif /* defined(__TrivialJokoa__ProfilaScene__) */

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
#include "cocosbuilder/CocosBuilder.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::network;

class ProfilaScene : public Scene
{
public:
    ProfilaScene();
	~ProfilaScene();
	virtual bool init();
    
	// implement the "static node()" method manually
	CREATE_FUNC(ProfilaScene);
private:
    SQLHelper *pSQL;
    Layer *pSailkapenaLayer;
    LayerColor *layer;
    
    Label *pAbisua;
    
    void backButton(Ref* sender);
    void loginCallback(Ref* pSender, cocos2d::extension::Control::EventType pCCControlEvent);
    void reset(Ref* sender);
    void onHttpRequestCompleted(HttpClient* client, HttpResponse* response);
};

class ProfilaLayerLoader : public cocosbuilder::LayerLoader
{
public:
//    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ProfilaLayerLoader, loader);
};



#endif /* defined(__TrivialJokoa__ProfilaScene__) */

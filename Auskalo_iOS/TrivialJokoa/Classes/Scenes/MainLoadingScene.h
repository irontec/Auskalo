//
//  MainLoadingScene.h
//  TrivialJokoa
//
//  Created by iker on 11/12/12.
//
//

#ifndef __TrivialJokoa__MainLoadingScene__
#define __TrivialJokoa__MainLoadingScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TrivialAPI.h"


class MainLoadingScene : public cocos2d::CCScene
{
public:
    MainLoadingScene();
	~MainLoadingScene();
	virtual bool init();
    void finishLoading();
    
	// implement the "static node()" method manually
	CREATE_FUNC(MainLoadingScene);
    
private:
    TrivialAPI *_api;
};

class MainLoadingLayerLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainLoadingLayerLoader, loader);
};


#endif /* defined(__TrivialJokoa__MainLoadingScene__) */

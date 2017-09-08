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
#include "../Helpers/TrivialAPI.h"

using namespace cocos2d;

class MainLoadingScene : public Scene
{
public:
    MainLoadingScene();
	~MainLoadingScene();
    
	virtual bool init();
    CREATE_FUNC(MainLoadingScene);
    
private:
    TrivialAPI *_api;
    void finishLoading();
    void updateProgress();
    Sprite *loading;
};

#endif /* defined(__TrivialJokoa__MainLoadingScene__) */

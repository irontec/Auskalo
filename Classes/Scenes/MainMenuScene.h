//
//  MainMenuScene.h
//  TrivialJokoa
//
//  Created by iker on 12/12/12.
//
//

#ifndef __TrivialJokoa__MainMenuScene__
#define __TrivialJokoa__MainMenuScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocosbuilder/CCBReader.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d;

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
	~MainMenuScene();
    
	virtual bool init();
	CREATE_FUNC(MainMenuScene);
private:
    void menuSelector(Ref* pSender);
    cocosbuilder::CCBReader *pReader;
    ControlButton *pJokatu, *pSailkapena, *pErantzuna3, *pHoniBuruz;
};

#endif /* defined(__TrivialJokoa__MainMenuScene__) */

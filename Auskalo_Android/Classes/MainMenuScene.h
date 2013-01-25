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

USING_NS_CC;
USING_NS_CC_EXT;

class MainMenuScene : public cocos2d::CCScene
{
public:
    MainMenuScene();
	~MainMenuScene();
	virtual bool init();
    
	static cocos2d::CCScene* scene();
    
	// implement the "static node()" method manually
	CREATE_FUNC(MainMenuScene);
private:
    CCBReader *pReader;
};

#endif /* defined(__TrivialJokoa__MainMenuScene__) */

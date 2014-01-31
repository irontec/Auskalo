//
//  CategorySelectorScene.h
//  TrivialJokoa
//
//  Created by iker on 13/12/12.
//
//

#ifndef __TrivialJokoa__CategoryScene__
#define __TrivialJokoa__CategoryScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Helpers/CCScrollLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CategoryScene : public CCScene, public CCScrollLayerDelegate
{
public:
    CategoryScene();
	~CategoryScene();
	virtual bool init();
    
	CREATE_FUNC(CategoryScene);
    
private:
    CCArray *pCategoriesButtons;
    CCArray *kategoriak;
    CCPoint menuPosition;
    CCPoint m_tBeginPos;
    
    CCArray *m_pBolasSprites;
    
    void loadLevels(CCObject *sender, CCControlEvent controlEvent);
    void backButton(CCObject *sender, CCControlEvent controlEvent);
    
    void scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page);
    
    CCArray* getKategoriak();
};

#endif /* defined(__TrivialJokoa__CategoryScene__) */

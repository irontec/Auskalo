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
#include "../Helpers/ScrollLayer.h"
#include "../Models/KategoriaModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CategoryScene : public Scene, public ScrollLayerDelegate
{
public:
    CategoryScene();
    ~CategoryScene();
    virtual bool init();
    
    CREATE_FUNC(CategoryScene);
    
private:
    Vector<KategoriaModel*> * kategoriak;
    Point menuPosition;
    Point m_tBeginPos;
    
    Vector<Sprite*> * m_pBolasSprites;
    
    void loadLevels(Ref* sender);
    void backButton(Ref* sender);
    
    void scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page);
};


#endif /* defined(__TrivialJokoa__CategoryScene__) */

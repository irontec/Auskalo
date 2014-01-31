//
//  SpriteButton.h
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 10/01/13.
//
//

#ifndef __TrivialJokoa__SpriteButton__
#define __TrivialJokoa__SpriteButton__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

typedef enum tagSpriteState
{
    kSpriteStateGrabbed,
    kSpriteStateUngrabbed,
    kSpriteStateDisabled
} SpriteState;

class SpriteButton : public CCSprite, public CCTargetedTouchDelegate
{
    SpriteState        m_state;
    float scale;
    
public:
    SpriteButton();
	~SpriteButton();
    
    CCRect rect();
    static SpriteButton* create(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector);
    static SpriteButton* create(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector, float scale);
    bool initWithSprite(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector, float scale);
    bool containsTouchLocation(CCTouch* touch);
    
    void setEnabled(bool enabled);
    
	virtual void onExit();
    virtual void onEnter();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    
protected:
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_Sprite, Sprite);
    
protected:
    CCObject*       m_pListener;
    SEL_CallFuncO    m_pfnSelector;
};

#endif /* defined(__TrivialJokoa__SpriteButton__) */

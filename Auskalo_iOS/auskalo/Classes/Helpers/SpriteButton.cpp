//
//  SpriteButton.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 10/01/13.
//
//

#include "SpriteButton.h"

#define kZoomActionTag 100

SpriteButton::SpriteButton()
{
    
}

SpriteButton::~SpriteButton()
{
    
}

CCRect SpriteButton::rect()
{
    CCSize s = getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

SpriteButton* SpriteButton::create(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector, float scale)
{
    SpriteButton *pRet = new SpriteButton();
    
    pRet->scale = scale;
    
    if (pRet && pRet->initWithSprite(aTexture, rec, selector, scale))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

SpriteButton* SpriteButton::create(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector)
{
    return create(aTexture, rec, selector, 1.0f);
}

bool SpriteButton::initWithSprite(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector, float scale)
{
    if (scale != 1.0f) {
        setScale(scale);
    }
    
    setContentSize(CCSizeMake(aTexture->getContentSize().width, aTexture->getContentSize().height));
    
    if(CCSprite::initWithTexture(aTexture) )
    {
        m_state = kSpriteStateUngrabbed;
    }
    
    m_pListener = rec;
    m_pfnSelector = selector;
    
    return true;
}

void SpriteButton::setEnabled(bool enabled)
{
    if (enabled) {
        m_state = kSpriteStateUngrabbed;
    } else {
        m_state = kSpriteStateDisabled;
    }
}

void SpriteButton::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

void SpriteButton::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSprite::onEnter();
}

bool SpriteButton::containsTouchLocation(CCTouch* touch)
{
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool SpriteButton::ccTouchBegan(CCTouch *touch, CCEvent *pEvent)
{
    
    if (m_state != kSpriteStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    m_state = kSpriteStateGrabbed;
    
    CCAction *action =getActionByTag(kZoomActionTag);
    
    if (action)
    {
        stopAction(action);
    }
    
    float scaleValue = 1.05f * scale;
    CCAction *zoomAction = CCScaleTo::create(0.05f, scaleValue);
    zoomAction->setTag(kZoomActionTag);
    runAction(zoomAction);
    
    return true;
}


void SpriteButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_state = kSpriteStateUngrabbed;
    
    CCAction *action = getActionByTag(kZoomActionTag);
    
    if (action)
    {
        stopAction(action);
    }
    
    float scaleValue = 1.0f * scale;
    
    CCAction *zoomAction = CCScaleTo::create(0.05f, scaleValue);
    zoomAction->setTag(kZoomActionTag);
    runAction(zoomAction);
    
    (m_pListener->*m_pfnSelector)(this);
}

void SpriteButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_state == kSpriteStateGrabbed)
    {
        float scaleValue = 1.0f * scale;
        CCAction *zoomAction = CCScaleTo::create(0.05f, scaleValue);
        zoomAction->setTag(kZoomActionTag);
        runAction(zoomAction);
        
        m_state = kSpriteStateUngrabbed;
    };
}

void SpriteButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void SpriteButton::touchDelegateRetain()
{
    this->retain();
}

void SpriteButton::touchDelegateRelease()
{
    this->release();
}
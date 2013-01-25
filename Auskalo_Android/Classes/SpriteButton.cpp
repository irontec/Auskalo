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

SpriteButton* SpriteButton::create(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector)
{
    SpriteButton *pRet = new SpriteButton();
    
    if (pRet && pRet->initWithSprite(aTexture, rec, selector))
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

bool SpriteButton::initWithSprite(CCTexture2D *aTexture, CCObject *rec, SEL_CallFuncO selector)
{    
    setContentSize(CCSizeMake(aTexture->getContentSize().width, aTexture->getContentSize().height));
    
    if(CCSprite::initWithTexture(aTexture) )
    {
        m_state = kSpriteStateUngrabbed;
    }
    
    m_pListener = rec;
    m_pfnSelector = selector;
    
    return true;
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
    
    float scaleValue = 1.1f;
    CCAction *zoomAction = CCScaleTo::create(0.05f, scaleValue);
    zoomAction->setTag(kZoomActionTag);
    runAction(zoomAction);
        
    return true;
}


void SpriteButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCAction *action = getActionByTag(kZoomActionTag);
    
    if (action)
    {
        stopAction(action);
    }
    
    float scaleValue = 1.0f;
    CCAction *zoomAction = CCScaleTo::create(0.05f, scaleValue);
    zoomAction->setTag(kZoomActionTag);
    runAction(zoomAction);
      
    (m_pListener->*m_pfnSelector)(this);
    
    m_state = kSpriteStateUngrabbed;
}

void SpriteButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_state == kSpriteStateGrabbed)
    {   
        float scaleValue = 1.0f;
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
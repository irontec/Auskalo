//
//  SpriteButton.cpp
//
//
//  Created by Sergio Garcia on 30/01/15.
//
//

#include "./SpriteButton.h"

#define kZoomActionTag 111
#define kDefaultZoomPercentage 5

SpriteButton::SpriteButton() {}

SpriteButton::~SpriteButton() {}

SpriteButton* SpriteButton::create(std::string filename, float scale, const ccMenuCallback& callback)
{
    return SpriteButton::create(filename, scale, kDefaultZoomPercentage, callback);
}

SpriteButton* SpriteButton::create(std::string filename, float scale, float zoomPercentage, const ccMenuCallback& callback)
{
    SpriteButton* pSprite = new SpriteButton();
    
    if (pSprite->initWithFile(filename))
    {
        pSprite->m_state = kSpriteStateUngrabbed;
        pSprite->scale = scale;
        pSprite->callback = callback;
        zoomPercentage = zoomPercentage / 100;
        pSprite->zoomPercentage = zoomPercentage;
        pSprite->autorelease();
        
        pSprite->initOptions();
        
        auto listener = pSprite->addEvents();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pSprite);
        
        return pSprite;
    }
    
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void SpriteButton::initOptions()
{
    if (scale != 1.0f) {
        setScale(scale);
    }
}

void SpriteButton::setScale(float fScale)
{
    scale = fScale;
    Sprite::setScale(fScale);
}

void SpriteButton::setEnabled(bool enabled)
{
    if (enabled) {
        m_state = kSpriteStateUngrabbed;
    } else {
        m_state = kSpriteStateDisabled;
    }
}

EventListenerTouchOneByOne *SpriteButton::addEvents()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = [&](cocos2d::Touch* touch, Event* event)
    {
        return SpriteButton::touchBegan(touch, event);
    };
    
    listener->onTouchMoved = [=](Touch* touch, Event* event)
    {
        SpriteButton::touchMoved(touch, event);
    };
    
    listener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        SpriteButton::touchEnded(touch, event);
    };
    
    listener->onTouchCancelled = [=](Touch* touch, Event* event)
    {
        SpriteButton::touchCancelled(touch, event);
    };
    
    return listener;
}

bool SpriteButton::touchBegan(Touch *touch, Event *pEvent)
{
    Vec2 touchLocation = getParent()->convertTouchToNodeSpace(touch);
    Rect rect = this->getBoundingBox();
    
    if(m_state != kSpriteStateUngrabbed
       || !rect.containsPoint(touchLocation))
        return false;
    
    m_state = kSpriteStateGrabbed;
    
    Action *action = getActionByTag(kZoomActionTag);
    
    if (action)
    {
        stopAction(action);
    }
    
    float scaleValue = 1.05f * scale;
    Action *zoomAction = ScaleTo::create(zoomPercentage, scaleValue);
    zoomAction->setTag(kZoomActionTag);
    runAction(zoomAction);
    
    return true;
}

void SpriteButton::touchEnded(Touch *touch, Event *pEvent)
{
    m_state = kSpriteStateUngrabbed;
    
    Action *action = getActionByTag(kZoomActionTag);
    
    if (action)
    {
        stopAction(action);
    }
    
    float scaleValue = 1.0f * scale;
    
    Action *zoomAction = ScaleTo::create(zoomPercentage, scaleValue);
    zoomAction->setTag(kZoomActionTag);
    runAction(zoomAction);
    
    Vec2 touchLocation = getParent()->convertTouchToNodeSpace(touch);
    Rect rect = this->getBoundingBox();
    
    if(rect.containsPoint(touchLocation) && callback != nullptr) {
        callback(this);
    }
}

void SpriteButton::touchMoved(Touch *touch, Event *pEvent)
{
    Vec2 touchLocation = getParent()->convertTouchToNodeSpace(touch);
    Rect rect = this->getBoundingBox();
    
    if(m_state == kSpriteStateGrabbed
       && !rect.containsPoint(touchLocation))
    {
        float scaleValue = 1.0f * scale;
        Action *zoomAction = ScaleTo::create(zoomPercentage, scaleValue);
        zoomAction->setTag(kZoomActionTag);
        runAction(zoomAction);
        
        m_state = kSpriteStateUngrabbed;
    };
}

void SpriteButton::touchCancelled(Touch *pTouch, Event *pEvent)
{
    log("SpriteButton - touchCancelled");
}

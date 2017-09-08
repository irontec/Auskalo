//
//  SpriteButton.h
//
//
//  Created by Sergio Garcia on 30/01/15.
//
//

#ifndef __SpriteButton__
#define __SpriteButton__

#include "cocos2d.h"
using namespace cocos2d;

typedef enum tagSpriteState
{
    kSpriteStateGrabbed,
    kSpriteStateUngrabbed,
    kSpriteStateDisabled
} SpriteState;

class SpriteButton : public Sprite
{
    SpriteState m_state;
    float scale;
    float zoomPercentage;
    
public:
    SpriteButton();
    ~SpriteButton();
    
    static SpriteButton* create(std::string filename, float scale, const ccMenuCallback& callback);
    static SpriteButton* create(std::string filename, float scale, float zoomPercentage, const ccMenuCallback& callback);
    
    void initOptions();
    
    void setEnabled(bool enabled);
    virtual void setScale(float fScale);
    EventListenerTouchOneByOne *addEvents();
    
    virtual bool touchBegan(Touch *touch, Event *pEvent);
    virtual void touchEnded(Touch *touch, Event *pEvent);
    virtual void touchMoved(Touch *touch, Event *pEvent);
    virtual void touchCancelled(Touch *pTouch, Event *pEvent);
protected:
    ccMenuCallback callback;
private:
    
};

#endif /* defined(___SpriteButton__) */

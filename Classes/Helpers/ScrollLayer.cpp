#include "ScrollLayer.h"
//#include "CCTouchHandler.h" // TODO: Check this

#include <assert.h>

namespace cocos2d
{
    enum
    {
        kCCScrollLayerStateIdle,
        kCCScrollLayerStateSliding,
    };
    
    ScrollLayer::ScrollLayer()
    : m_pDelegate(NULL), m_pLayers(NULL), m_bStealingTouchInProgress(false), m_pScrollTouch(NULL), m_iState(kCCScrollLayerStateIdle)
    
    {
    }
    
    ScrollLayer::~ScrollLayer()
    {
        m_pDelegate = NULL;
    }
    
    unsigned int ScrollLayer::getTotalScreens() const
    {
        return (int) m_pLayers->size();
    }
    
    ScrollLayer* ScrollLayer::nodeWithLayers(Vector<Layer *> *layers, int widthOffset)
    {
        ScrollLayer* pRet = new ScrollLayer();
        if (pRet && pRet->initWithLayers(layers, widthOffset))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    
    bool ScrollLayer::initWithLayers(Vector<Layer *> *layers, int widthOffset)
    {
        if (!Layer::init())
            return false;
        CC_ASSERT(layers && layers->size());
        
        setTouchEnabled(true);
        
        m_bStealTouches = true;
        
        // Set default minimum touch length to scroll.
        m_fMinimumTouchLengthToSlide = 30.0f;
        m_fMinimumTouchLengthToChangePage = 100.0f;
        
        m_fMarginOffset = Director::getInstance()->getWinSize().width;
        
        // Show indicator by default.
        m_bShowPagesIndicator = true;
        m_tPagesIndicatorPosition = Vec2(0.5f * _contentSize.width, ceilf(_contentSize.height / 8.0f));
        
        // Set up the starting variables
        m_uCurrentScreen = 0;
        
        // Save offset.
        m_fPagesWidthOffset = (float)widthOffset;
        
        // Save array of layers.
        m_pLayers = new Vector<Layer *>();
        m_pLayers->pushBack(*layers);
        
        
        updatePages();
        
        return true;
    }
    
    void ScrollLayer::updatePages()
    {
        // Loop through the array and add the screens if needed.
        for (int i = 0; i < (int) m_pLayers->size(); i++) {
            auto layer = m_pLayers->at(i);
            layer->setAnchorPoint(Vec2(0,0));
            layer->setContentSize(Director::getInstance()->getWinSize());
            layer->setPosition(Vec2((i * (_contentSize.width - m_fPagesWidthOffset)), 0));
            if (!layer->getParent())
                addChild(layer);
        }
    }
    
    // Layer Methods ReImpl
    /* TODO: Check this
     void ScrollLayer::visit()
     {
     Layer::visit();	//< Will draw after glPopScene.
     }
     */
    // Moving To / Selecting Pages
    
    void ScrollLayer::moveToPageEnded()
    {
        if (m_pDelegate)
            m_pDelegate->scrollLayerScrolledToPageNumber(this, m_uCurrentScreen);
    }
    
    unsigned int ScrollLayer::pageNumberForPosition(const Point& position)
    {
        float pageFloat = - _position.x / (_contentSize.width - m_fPagesWidthOffset);
        int pageNumber = (int)ceilf(pageFloat);
        if ((float)pageNumber - pageFloat  >= 0.5f)
            pageNumber--;
        
        pageNumber = MAX(0, pageNumber);
        pageNumber = MIN((int) m_pLayers->size() - 1, pageNumber);
        
        return (unsigned int)pageNumber;
    }
    
    
    Point ScrollLayer::positionForPageWithNumber(unsigned int pageNumber)
    {
        return Vec2(pageNumber * -1.f * (_contentSize.width - m_fPagesWidthOffset), 0.0f);
    }
    
    void ScrollLayer::moveToPage(unsigned int pageNumber)
    {
        if (pageNumber >= (int) m_pLayers->size())
        {
            CCLOGERROR("ScrollLayer::moveToPage: %d - wrong page number, out of bounds.", pageNumber);
            return;
        }
        
        //auto callback = CC_CALLBACK_1(ScrollLayer::moveToPageEnded, this);
        auto callback = CallFunc::create( [this]() {
            this->moveToPageEnded();
        });
        
        auto changePage = Sequence::create(
                                           MoveTo::create(0.3f, positionForPageWithNumber(pageNumber)),
                                           callback,
                                           nullptr
                                           );
        runAction(changePage);
        m_uCurrentScreen = pageNumber;
    }
    
    void ScrollLayer::selectPage(unsigned int pageNumber)
    {
        if (pageNumber >= (int) m_pLayers->size())
        {
            CCLOGERROR("ScrollLayer::selectPage: %d - wrong page number, out of bounds.", pageNumber);
            return;
        }
        
        setPosition(positionForPageWithNumber(pageNumber));
        m_uCurrentScreen = pageNumber;
        
    }
    
    // Dynamic Pages Control
    
    void ScrollLayer::addPage(Layer* aPage)
    {
        addPage(aPage, (int) m_pLayers->size());
    }
    
    void ScrollLayer::addPage(Layer* aPage, unsigned int pageNumber)
    {
        pageNumber = MIN(pageNumber, (int) m_pLayers->size());
        pageNumber = MAX(pageNumber, 0);
        
        m_pLayers->insert(pageNumber, aPage);
        
        updatePages();
        moveToPage(m_uCurrentScreen);
    }
    
    void ScrollLayer::removePage(Layer* aPage)
    {
        m_pLayers->eraseObject(aPage);
        removeChild(aPage, true);
        
        updatePages();
        
        m_uCurrentScreen = MIN(m_uCurrentScreen, (int) m_pLayers->size() - 1);
        moveToPage(m_uCurrentScreen);
    }
    
    void ScrollLayer::removePageWithNumber(unsigned int pageNumber)
    {
        if (pageNumber < (int) m_pLayers->size())
            removePage((m_pLayers->at(pageNumber)));
    }
    
    // Touches
    
    // Register with more priority than CCMenu's but don't swallow touches
    //	void ScrollLayer::registerWithTouchDispatcher()
    //	{
    //		CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority - 1, false);
    //	}
    void ScrollLayer::onEnter()
    {
        Layer::onEnter();
        //		CCDirector* pDirector = Director::getInstance();
        //        pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(false);
        
        listener->onTouchBegan = [&](cocos2d::Touch* touch, Event* event)
        {
            return ScrollLayer::touchBegan(touch, event);
        };
        
        listener->onTouchMoved = [=](Touch* touch, Event* event)
        {
            ScrollLayer::touchMoved(touch, event);
        };
        
        listener->onTouchEnded = [=](Touch* touch, Event* event)
        {
            ScrollLayer::touchEnded(touch, event);
        };
        
        listener->onTouchCancelled = [=](Touch* touch, Event* event)
        {
            ScrollLayer::touchCancelled(touch, event);
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    void ScrollLayer::onExit()
    {
        //		CCDirector* pDirector = Director::getInstance();
        //        pDirector->getTouchDispatcher()->removeDelegate(this);
        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
        Layer::onExit();
    }
    
    
    /** Hackish stuff - stole touches from other CCTouchDispatcher targeted delegates.
     Used to claim touch without receiving ccTouchBegan. */
    // TODO: Check this
    /*
     void ScrollLayer::claimTouch(Touch* pTouch)
     {
     CCTargetedTouchHandler* handler = (CCTargetedTouchHandler*)Director::getInstance()->getTouchDispatcher()->findHandler(this);
     if (handler)
     {
     CCSet* claimedTouches = handler->getClaimedTouches();
     if (!claimedTouches->containsObject(pTouch))
     {
     claimedTouches->addObject(pTouch);
     }
     else
     {
     CCLOGERROR("ScrollLayer::claimTouch is already claimed!");
     }
     }
     }
     
     void ScrollLayer::cancelAndStoleTouch(Touch* pTouch, Event* pEvent)
     {
     // Throw Cancel message for everybody in TouchDispatcher.
     CCSet* touchSet = new CCSet();
     touchSet->addObject(pTouch);
     touchSet->autorelease();
     m_bStealingTouchInProgress = true;
     Director::getInstance()->getTouchDispatcher()->touchesCancelled(touchSet, pEvent);
     m_bStealingTouchInProgress = false;
     
     //< after doing this touch is already removed from all targeted handlers
     
     // Squirrel away the touch
     claimTouch(pTouch);
     }
     */
    void ScrollLayer::touchCancelled(Touch* pTouch, Event* pEvent)
    {
        // Do not cancel touch, if this method is called from cancelAndStoleTouch:
        if (m_bStealingTouchInProgress)
            return;
        
        if (m_pScrollTouch == pTouch)
        {
            m_pScrollTouch = NULL;
            selectPage(m_uCurrentScreen);
        }
    }
    
    bool ScrollLayer::touchBegan(Touch* pTouch, Event* pEvent)
    {
        if (!m_pScrollTouch)
            m_pScrollTouch = pTouch;
        else
            return false;
        
        Point touchPoint = pTouch->getLocationInView();
        touchPoint = Director::getInstance()->convertToGL(touchPoint);
        
        m_fStartSwipe = touchPoint.x;
        m_iState = kCCScrollLayerStateIdle;
        
        return true;
    }
    
    void ScrollLayer::touchMoved(Touch* pTouch, Event* pEvent)
    {
        if(m_pScrollTouch != pTouch)
            return;
        
        Point touchPoint = pTouch->getLocationInView();
        touchPoint = Director::getInstance()->convertToGL(touchPoint);
        
        // If finger is dragged for more distance then minimum - start sliding and cancel pressed buttons.
        // Of course only if we not already in sliding mode
        if ((m_iState != kCCScrollLayerStateSliding)
            && (fabsf(touchPoint.x - m_fStartSwipe) >= m_fMinimumTouchLengthToSlide))
        {
            m_iState = kCCScrollLayerStateSliding;
            
            // Avoid jerk after state change.
            m_fStartSwipe = touchPoint.x;
            
            // TODO: Check this
            //			if (m_bStealTouches)
            //				cancelAndStoleTouch(pTouch, pEvent);
            
            if (m_pDelegate)
                m_pDelegate->scrollLayerScrollingStarted(this);
        }
        
        if (m_iState == kCCScrollLayerStateSliding)
        {
            float desiredX = (m_uCurrentScreen * -1.f * (_contentSize.width - m_fPagesWidthOffset)) + touchPoint.x - m_fStartSwipe;
            unsigned int page = pageNumberForPosition(Vec2(desiredX, 0));
            float offset = desiredX - positionForPageWithNumber(page).x;
            if ((page == 0 && offset > 0) || (page == (int) m_pLayers->size() - 1 && offset < 0))
                offset -= m_fMarginOffset * (offset / Director::getInstance()->getWinSize().width);
            else
                offset = 0;
            setPosition(Vec2(desiredX - offset, 0));
        }
    }
    
    void ScrollLayer::touchEnded(Touch* pTouch, Event* pEvent)
    {
        if(m_pScrollTouch != pTouch)
            return;
        
        m_pScrollTouch = NULL;
        
        Point touchPoint = pTouch->getLocationInView();
        touchPoint = Director::getInstance()->convertToGL(touchPoint);
        
        unsigned int selectedPage = m_uCurrentScreen;
        float delta = touchPoint.x - m_fStartSwipe;
        if (fabs(delta) >= m_fMinimumTouchLengthToChangePage)
        {
            selectedPage = pageNumberForPosition(_position);
            if (selectedPage == m_uCurrentScreen)
            {
                if (delta < 0.f && selectedPage < (int) m_pLayers->size() - 1)
                    selectedPage++;
                else if (delta > 0.f && selectedPage > 0)
                    selectedPage--;
            }
        }
        moveToPage(selectedPage);
    }
}

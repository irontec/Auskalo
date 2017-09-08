#include "SlidingMenu.h"

SlidingMenuGrid::SlidingMenuGrid() 
{
	
}

SlidingMenuGrid::~SlidingMenuGrid() 
{
	
}

void SlidingMenuGrid::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeAllEventListeners();
}

bool SlidingMenuGrid::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	return true;
}

SlidingMenuGrid* SlidingMenuGrid::menuWithArray(Vector<Ref*> *items, int cols, int rows, Point pos , Point pad)
{ 
	return menuWithArray(items,cols,rows,pos,pad,false);
}


SlidingMenuGrid* SlidingMenuGrid::menuWithArray(Vector<Ref*> *items, int cols, int rows, Point pos , Point pad, bool vertical)
{
	SlidingMenuGrid *slidingMenu = new SlidingMenuGrid();
	if (slidingMenu && slidingMenu->initWithArray(items ,cols ,rows ,pos ,pad ,vertical))
	{
		slidingMenu->autorelease();
		return slidingMenu;
	}
	CC_SAFE_DELETE(slidingMenu);
	return NULL;
}

bool SlidingMenuGrid::initWithArray(Vector<Ref*> *items, int cols, int rows, Point pos , Point pad, bool vertical)
{
    if( !Layer::init() )
    {
        return false;
    }
    
    selectedItem = NULL;
    setTouchEnabled(true);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(SlidingMenuGrid::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SlidingMenuGrid::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SlidingMenuGrid::onTouchEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    pMenu = new Menu;
    addChild(pMenu,0);
    
    MenuItemSprite *getItem;
    
    for (auto object: *items)
    {
        getItem = (MenuItemSprite*)object;
        pMenu->addChild(getItem, 1, getItem->getTag());
    }
    
    padding = pad;
    iCurrentPage = 0;
    bMoving = false;
    iMenuRows=rows;
    menuOrigin = pos;
    fMoveDeadZone = 10;
    bVerticalPaging = vertical;
    fAnimSpeed = 1;
    state=Menu::State::WAITING;
    (bVerticalPaging) ? this->buildGridVertical(cols ,rows) : this->buildGrid(cols, rows);
    this->setPosition(menuOrigin);
    return true;
}


void SlidingMenuGrid::buildGrid(int cols, int rows)
{
	Size winSize = Director::getInstance()->getWinSize();

	int col = 0, row = 0;
	Vector<Node*> childs = pMenu->getChildren();
	Point position=getPosition();
	iPageCount=0;
    for (auto item: childs)
    {
        MenuItemSprite* getItem = (MenuItemSprite*) item;
        getItem->setPosition(Vec2(position.x +  menuOrigin.x + col * padding.x + (iPageCount * winSize.width), position.y +(winSize.height-menuOrigin.y)- row * padding.y));
        ++col;
        if (col == cols)
        {
            col = 0;
            ++row;
            if( row == rows )
            {
                iPageCount++;
                col = 0;
                row = 0;
            }
        }
    }

    if(childs.size() > rows*cols*iPageCount)//   <-- add code for FIX (Mr.  K pop)
	{
		iPageCount++;
	}	
}



void SlidingMenuGrid::buildGridVertical(int cols, int rows)
{ 
	Size winSize = Director::getInstance()->getWinSize();

	int col = 0, row = 0;
	Vector<Node*> childs = pMenu->getChildren();
	Point position=getPosition();
	iPageCount=0;
    for (auto item: childs)
    {
        MenuItemSprite* getItem = (MenuItemSprite*) item;
        getItem->setPosition(Vec2(position.x + menuOrigin.x+col * padding.x , position.y +(winSize.height-menuOrigin.y)- row * padding.y - (iPageCount * winSize.height)));
        ++col;
        if (col == cols)
        {
            col = 0;
            ++row;
            if( row == rows )
            {
                iPageCount++;
                col = 0;
                row = 0;
            }
        }
    }

	if(childs.size() > rows*cols*iPageCount)//   <-- add code for FIX (Mr.  K pop)
	{
		iPageCount++;
	}
}


MenuItemSprite* SlidingMenuGrid::GetItemWithinTouch(Touch* touch)
{
    Point touchLocation = Director::getInstance()->convertToGL(touch->getLocationInView());

	// Parse our menu items and see if our touch exists within one.
	Vector<Node*> childs = pMenu->getChildren();
    for (auto item: childs)
    {
        MenuItemSprite * getItem= static_cast<MenuItemSprite *>(item);
        
        Point local = getItem->convertToNodeSpace(touchLocation);
        Rect r = getItem->rect();
        r.origin = Point::ZERO;// If the touch was within this item. Return the item.
        
        Rect *pPoint = new Rect(r);
        
        if (pPoint->containsPoint(local))
        {
            return getItem;
        }
        
        CC_SAFE_DELETE(pPoint);
    }

	// Didn't touch an item. 
	return NULL;
}

bool SlidingMenuGrid::onTouchBegan(Touch* touch, Event* event)
{
    touchOrigin = Director::getInstance()->convertToGL(touch->getLocationInView());

    // If we weren't in "waiting" state bail out.
	if (state == Menu::State::WAITING)
	{ 
		// Activate the menu item if we are touching one.
		if(!bMoving)
		{
			selectedItem = GetItemWithinTouch(touch);
			if(selectedItem)
			{
				if(selectedItem->isEnabled())
					selectedItem->selected();
			}
		}
		state = Menu::State::TRACKING_TOUCH;
	}
	return true;
}

void SlidingMenuGrid::onTouchCancelled(Touch* touch, Event* event)
{
	if(selectedItem)
	{
		selectedItem->unselected();
		selectedItem = NULL;
		state = Menu::State::WAITING;
	}
}

void SlidingMenuGrid::onTouchMoved(Touch* touch, Event* event)
{
	if(GetItemWithinTouch(touch) == NULL && selectedItem)
	{
		//Touch Cancelled
		if(selectedItem->isEnabled())
		{
			selectedItem->unselected();
		}
		selectedItem = NULL;
		state = Menu::State::WAITING; 
		return;
	}

	if(GetItemWithinTouch(touch) != NULL && selectedItem)
	{
		return;
	}
	// Calculate the current touch point during the move.
    touchStop = Director::getInstance()->convertToGL(touch->getLocationInView());// Distance between the origin of the touch and current touch point.
	fMoveDelta = (bVerticalPaging) ? (touchStop.y - touchOrigin.y) : (touchStop.x - touchOrigin.x);// Set our position.    
	setPosition(GetPositionOfCurrentPageWithOffset(fMoveDelta));
	bMoving = true;
}

void SlidingMenuGrid::onTouchEnded(Touch* touch, Event* event)
{
	if( bMoving )
	{
		bMoving = false;
		{			
			// Do we have multiple pages?
			if( iPageCount > 1 && (fMoveDeadZone < std::abs(fMoveDelta)))
			{
				// Are we going forward or backward?
				bool bForward = (fMoveDelta > 0) ? true : false;
				
				// Do we have a page available?
				if(bVerticalPaging)
				{
					if(bForward && (iPageCount>iCurrentPage+1))
					{
						// Increment currently active page.
						iCurrentPage++;
					}
					else if(!bForward && (iCurrentPage > 0))
					{
						// Decrement currently active page.
						iCurrentPage--;
					}
				}
				else
				{
					if(!bForward && (iPageCount>iCurrentPage+1))
					{
						// Increment currently active page.
						iCurrentPage++;
					}
					else if(bForward && (iCurrentPage > 0))
					{
						// Decrement currently active page.
						iCurrentPage--;
					}
				}
				
				// Start sliding towards the current page.
				moveToCurrentPage();
			}
			else 
			{
				if(selectedItem)
				{
					selectedItem->unselected();
					selectedItem->activate();
				}
			}				
		}
	}
	else
	{
		//Only touch
		if(selectedItem)
		{
			if(selectedItem->isEnabled())
			{
				selectedItem->unselected();
				selectedItem->activate();
			}
			//selectedItem=NULL;			
		}
	}
	state = Menu::State::WAITING;
}

void SlidingMenuGrid::moveToCurrentPage(bool animated)
{
	Size winSize = Director::getInstance()->getWinSize();

//    if (animated)
//    {
//        CCEaseBounce* action = CCEaseBounce::create(MoveTo::create(fAnimSpeed * 0.3f, GetPositionOfCurrentPage()));
//        runAction(action);
//    }
//    else
//    {
        setPosition(GetPositionOfCurrentPage());
//    }
}

Point SlidingMenuGrid::GetPositionOfCurrentPage()
{
	Size winSize = Director::getInstance()->getWinSize();
	Point position=getPosition();
	if(bVerticalPaging)
		position=Size(0,iCurrentPage*winSize.height);
	else
		position=Size(-(iCurrentPage)*winSize.width,0);
	return position;
}

Point SlidingMenuGrid::GetPositionOfCurrentPageWithOffset(float offset)
{
	Size winSize = Director::getInstance()->getWinSize();
	Point position=GetPositionOfCurrentPage();
	if(bVerticalPaging)
		position.y+=offset;
	else
		position.x+=offset;		
	return position;
}

// Returns the swiping dead zone. 
float SlidingMenuGrid::GetSwipeDeadZone()
{
	return fMoveDeadZone;
}

void SlidingMenuGrid::SetSwipeDeadZone(float fValue) 
{
	fMoveDeadZone = fValue;
}

// Returns wheather or not vertical paging is enabled.
bool SlidingMenuGrid::IsVerticallyPaged()
{
	return bVerticalPaging; 
}

// Sets the vertical paging flag.
void SlidingMenuGrid::SetVerticalPaging(bool bValue) 
{
	bVerticalPaging = bValue;
	// this->buildGridVertical();
}

void SlidingMenuGrid::gotoPage(int iPage, bool animated)
{
	int iSetPage=iPage;
	if(iPage<=0)
		iSetPage=1;
	else if(iPage>iPageCount)
	{
		iSetPage=iPageCount;
	}
	iCurrentPage=iSetPage-1;
	moveToCurrentPage(animated);
}

void SlidingMenuGrid::setPageLabel( int iPage,Node * pLabel)
{
	addChild(pLabel,10);
	if(!pLabel)
		return;
	int iSetPage=iPage;
	if(iPage<=0)
		iSetPage=1;
	else if(iPage>iPageCount)
	{
		iSetPage=iPageCount;
	}
	Size winSize = Director::getInstance()->getWinSize();
	Size labelSize= pLabel->getContentSize();
	Point pt=getPosition();
	if(bVerticalPaging)
		pt=Size(menuOrigin.x+labelSize.width/2,winSize.height-menuOrigin.y-winSize.height*(iSetPage-1)-iMenuRows*padding.y-labelSize.height/2);
	else
		pt=Size(menuOrigin.x+(iSetPage-1)*winSize.width+labelSize.width/2,winSize.height-menuOrigin.y-iMenuRows*padding.y-labelSize.height/2);
	pLabel->setPosition(pt);
}

#include "VisibleRect.h"

Rect VisibleRect::s_visibleRect;

void VisibleRect::lazyInit()
{
    if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
    {
        cocos2d::GLView* pEGLView = Director::getInstance()->getOpenGLView();
        s_visibleRect.origin = pEGLView->getVisibleOrigin();
        s_visibleRect.size = pEGLView->getVisibleSize();
    }
}

Rect VisibleRect::getVisibleRect()
{
    lazyInit();
    return Rect(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

Point VisibleRect::left()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::right()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::top()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::bottom()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

Point VisibleRect::center()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::leftTop()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::rightTop()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::leftBottom()
{
    lazyInit();
    return s_visibleRect.origin;
}

Point VisibleRect::rightBottom()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}

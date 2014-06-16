//
//  CCNodeExt.cpp
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#include "CCNodeExt.h"

NS_CC_BEGIN

DrawNodeExt * DrawNodeExt::create()
{
    DrawNodeExt * ret = new DrawNodeExt();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

void DrawNodeExt::drawRect(const Rect &rect, int stroke, const Color4F &borderColor)
{
    Point vert[] = {
        rect.origin,
        rect.origin + Point(0, rect.size.height),
        rect.origin + Point(rect.size),
        rect.origin + Point(rect.size.width, 0),
    };
    DrawNode::drawSegment(vert[0], vert[1], stroke, borderColor);
    DrawNode::drawSegment(vert[1], vert[2], stroke, borderColor);
    DrawNode::drawSegment(vert[2], vert[3], stroke, borderColor);
    DrawNode::drawSegment(vert[3], vert[0], stroke, borderColor);
}

void DrawNodeExt::fillRect(const Rect &rect, int stroke, const Color4F &borderColor, const Color4F &fillColor)
{
    Point vert[] = {
        rect.origin,
        rect.origin + Point(0, rect.size.height),
        rect.origin + Point(rect.size),
        rect.origin + Point(rect.size.width, 0),
    };
    DrawNode::drawPolygon(vert, 4, fillColor, stroke, borderColor);
}

NS_CC_END
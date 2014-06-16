//
//  CCSLayer.cpp
//  CocosManual
//
//  Created by Jiangy on 14/6/12.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "CCSLayer.h"

CREATER_FROM_CCS(CCSLayer, "ccs/UI/UI_1.ExportJson")

CCSLayer::~CCSLayer()
{
    // NOTE: destructor here
}

void CCSLayer::reset()
{

}

void CCSLayer::bindFromCCS()
{
    Layout * layout = static_cast<Layout *>(getChildByTag(CCS_FILE_TAG));
    CC_UNUSED_PARAM(layout);
    // BIND_WIDGET(layout, _name, _type, _widget) // layout maybe other parent widget
    // BIND_WIDGET_WITH_LISTENER_TOUCH(layout, _name, _type, _widget, _target, _listener)
    BIND_WIDGET(layout, "labHellos", Text *, labHellos);
    BIND_WIDGET_WITH_LISTENER_TOUCH(layout, "btnClick_0", Button *, btnClick_0, this, CCSLayer::onClickMe0);
    BIND_WIDGET_WITH_LISTENER_TOUCH(layout, "btnClick_1", Button *, btnClick_1, this, CCSLayer::onClickMe1);
    
    onBindFinish();
}

void CCSLayer::onBindFinish()
{
    CCLOG("CCSLayer load finish");
    addWithScene = false;
    // NOTE: initialization ccb items here
    
}

#pragma mark -

void CCSLayer::onClickMe0(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            labHellos->runAction(CCRotateBy::create(1, 360));
            break;
        }
        default:
            break;
    }
}

void CCSLayer::onClickMe1(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            labHellos->runAction(CCRotateBy::create(1, -360));
            break;
        }
        default:
            break;
    }
}


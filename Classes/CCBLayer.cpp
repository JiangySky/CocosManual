//
//  CCBLayer.cpp
//  CocosManual
//
//  Created by Jiangy on 14/6/4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "CCBLayer.h"

CREATER_FROM_CCB(CCBLayer, CCBLayerLoader, "MainScene.ccbi", "CCBLayer")

CCBLayer::CCBLayer()
{
    // NOTE: structor here
}

CCBLayer::~CCBLayer()
{
    // NOTE: destructor here
}

bool CCBLayer::init()
{
    // NOTE: initialization logic & ccbMember to NULL & regist other ccbLoader here
    loadCCBFinish = false;
    helloLabel = nullptr;
    
    return true;
}

#pragma mark - == CCB Loader func == -

SEL_MenuHandler CCBLayer::onResolveCCBCCMenuItemSelector(Ref * pTarget, const char * pSelectorName) {
    // NOTE: binding MenuItem selector here
    // CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSomething", CCBLayer::onSomething);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPressButton", CCBLayer::onPressButton);
    CCLOG("selector not binding: %s", pSelectorName);
    return NULL;
}

Control::Handler CCBLayer::onResolveCCBCCControlSelector(Ref * pTarget, const char * pSelectorName) {
    // NOTE: binding CCControl selector here
    // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSomething", CCBLayer::onSomething);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressButton", CCBLayer::onPressButton);
    CCLOG("selector not binding: %s", pSelectorName);
    return NULL;
}

bool CCBLayer::onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * pNode) {
    // NOTE: binding member here
    // CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "variable", Type *, this->variable);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "helloLabel", Label *, this->helloLabel);
    CCLOG("member not binding: %s", pMemberVariableName);
    return false;
}

void CCBLayer::onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader) {
    CC_UNUSED_PARAM(pNode);
    CC_UNUSED_PARAM(pNodeLoader);
    
    if (!loadCCBFinish) {
        loadCCBFinish = true;
        this->didLoadFromCCB();
    }
}

void CCBLayer::didLoadFromCCB() {
    CCLOG("CCBLayer load finish");
    addWithScene = false;
    // NOTE: initialization ccb items here
}

#pragma mark -

void CCBLayer::onPressButton(Ref *pSender)
{
    CCLOG("onPressButton by MenuItem");
    CC_UNUSED_PARAM(pSender);
    helloLabel->runAction(CCRotateBy::create(1, -360));
}

void CCBLayer::onPressButton(Ref *pSender, Control::EventType type)
{
    CCLOG("onPressButton by ControlButton");
    CC_UNUSED_PARAM(pSender);
    CC_UNUSED_PARAM(type);
    helloLabel->runAction(CCRotateBy::create(1, 360));
}


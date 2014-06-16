//
//  MainScene.cpp
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#include "MainScene.h"
#include "ManualDisplay.h"
#include "ManualCode.h"
#include "ManualList.h"

bool MainScene::init()
{
    Size sizeWin = DIRECTOR->getWinSize();
    float listWidth = sizeWin.width - sizeWin.height;
    
    // background layout
    auto layerColor = LayerColor::create(Color4B::WHITE, sizeWin.width, sizeWin.height);
    auto drawNodeV = DrawNode::create();
    drawNodeV->drawSegment(Point(listWidth, 0), Point(listWidth, sizeWin.height), 2, Color4F::GRAY);
    layerColor->addChild(drawNodeV);
    auto drawNodeH = DrawNode::create();
    drawNodeH->drawSegment(Point(listWidth, sizeWin.height / 3), Point(sizeWin.width, sizeWin.height / 3), 2, Color4F::GRAY);
    layerColor->addChild(drawNodeH);
    addChild(layerColor);
    
    // title
    auto btnTitle = Button::create();
    btnTitle->setTouchEnabled(true);
    btnTitle->ignoreContentAdaptWithSize(false);
    btnTitle->setTitleFontName("Marker Felt");
    btnTitle->setTitleText("Cocos2d Manual");
    btnTitle->setTitleColor(Color3B::GREEN);
    btnTitle->setTitleFontSize(30);
    btnTitle->setSize(Size(listWidth, 40));
    btnTitle->setPosition(Point(listWidth / 2, sizeWin.height - 30));
    BIND_LISTENER_TOUCH(btnTitle, this, MainScene::onTouchTitle);
    addChild(btnTitle);
    
    // manual layer
    auto layerDisplay = ManualDisplay::create();
    auto layerCode = ManualCode::create();
    auto layerList = ManualList::create();
    layerCode->setDisplay(layerDisplay);
    layerCode->setTag(Tag::TAG_CODELAYER);
    layerList->setCode(layerCode);
    addChild(layerDisplay);
    addChild(layerCode);
    addChild(layerList);
    
    return Layer::init();
}

void MainScene::onTouchTitle(cocos2d::Ref *pSender, TouchEventType type)
{
    CC_UNUSED_PARAM(pSender);
    
    switch (type) {
        case cocos2d::ui::TOUCH_EVENT_ENDED: {
            auto layerCode = getChildByTag(Tag::TAG_CODELAYER);
            CC_BREAK_IF(!layerCode);
            dynamic_cast<ManualCode *>(layerCode)->displayCode("Welcome");
            break;
        }
        default:
            break;
    }
}



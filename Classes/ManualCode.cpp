//
//  ManualCode.cpp
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#include "ManualCode.h"
#include "ManualDisplay.h"

bool ManualCode::init()
{
    Size sizeWin = DIRECTOR->getWinSize();
    float listWidth = sizeWin.width - sizeWin.height + 5;
    setContentSize(Size(sizeWin.width - listWidth, sizeWin.height / 3 - 5));
    setPosition(Point(listWidth, 0));
    
    // load code data
    mapCode = FILEUTILS->getValueMapFromFile("Code.plist");
    
    // scroll view
    codeView = ui::ScrollView::create();
    codeView->setDirection(SCROLLVIEW_DIR_VERTICAL);
    codeView->setBounceEnabled(true);
    codeView->setSize(getContentSize());
    codeView->setTag(Tag::TAG_SCROLLVIEW);
    codeView->setPosition(Point::ZERO);
    addChild(codeView);
    
    displayCode("Welcome");
    
    return true;
}

void ManualCode::addCodeTexture(string text) const
{
    float h = LabelDimHeight(text.c_str(), "Helvetica", 20, codeView->getSize().width);
    if (h < codeView->getSize().height) {
        h = codeView->getSize().height;
    }
    auto label = Label::createWithSystemFont(text.c_str(), "Helvetica", 20);
    label->disableEffect();
    label->setColor(Color3B::BLACK);
    label->setDimensions(codeView->getSize().width, h);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    
    codeView->removeAllChildrenWithCleanup(true);
    codeView->setInnerContainerSize(label->getDimensions());
    codeView->setInertiaScrollEnabled(true);
    codeView->addChild(label);
    
    codeView->jumpToTop();
}

void ManualCode::displayCode(const char *key)
{
    CCLOG("[=== Display : %s ===]", key);
    if (mapCode.find(key) != mapCode.end()) {
        addCodeTexture(mapCode.at(key).asString());
    }
    if (display) {
        display->runCode(key);
    }
}


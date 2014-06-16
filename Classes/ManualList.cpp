//
//  ManualList.cpp
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#include "ManualList.h"
#include "ManualCode.h"

bool ManualList::init()
{
    static bool listWithButton = true;
    Size sizeWin = DIRECTOR->getWinSize();
    float listWidth = sizeWin.width - sizeWin.height;
    setContentSize(Size(listWidth, sizeWin.height - 60));
    
    // list view
    auto listView = ListView::create();
    listView->setTag(Tag::TAG_LISTVIEW);
    listView->setDirection(SCROLLVIEW_DIR_VERTICAL);
    listView->setBounceEnabled(true);
    listView->ignoreContentAdaptWithSize(false);
    listView->setSize(getContentSize());
    
    // get list data
    auto listData = FILEUTILS->getValueVectorFromFile("List.plist");
    for (int i = 0; i < listData.size(); i++) {
        auto title = listData[i].asString();
        auto widget = Layout::create();
        
        if (listWithButton) {
            auto btnTitle = Button::create();
            btnTitle->setTouchEnabled(true);
            btnTitle->ignoreContentAdaptWithSize(false);
            btnTitle->setTitleText(title.c_str());
            btnTitle->setName(title.c_str());
            btnTitle->setTitleColor(Color3B::BLACK);
            btnTitle->setTitleFontSize(20);
            btnTitle->setSize(Size(listWidth, 40));
            btnTitle->setPosition(Point(listWidth / 2, 20));
            BIND_LISTENER_TOUCH(btnTitle, this, ManualList::onTouchItem);
            
            widget->setSize(btnTitle->getSize());
            widget->addChild(btnTitle);
            
        } else {
            auto labelTitle = Label::createWithSystemFont(title.c_str(), "Helvetica", 20);
            labelTitle->setColor(Color3B::BLACK);
            labelTitle->setContentSize(Size(listWidth, 40));
            labelTitle->setPosition(Point(listWidth / 2, 20));
            labelTitle->setTag(Tag::TAG_LABEL);
            
            widget->setSize(labelTitle->getContentSize());
            widget->addChild(labelTitle);
        }
        
        listView->pushBackCustomItem(widget);
    }
    
    listView->setPosition(Point::ZERO);
    if (!listWithButton) {
        listView->setTouchEnabled(true);
        listView->addEventListenerListView(this, listvieweventselector(ManualList::onListviewEvent));
    }
    addChild(listView);
    
    return true;
}

void ManualList::onTouchItem(Button * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED:
            if (code) {
                code->displayCode(pSender->getName());
            }
            break;
            
        default:
            break;
    }
}

void ManualList::onListviewEvent(ListView *pSender, ListViewEventType type)
{
    switch (type) {
        case LISTVIEW_ONSELECTEDITEM_END: {
            auto widget = pSender->getItem(pSender->getCurSelectedIndex());
            CC_BREAK_IF(!widget);
            Label * label = dynamic_cast<Label *>(widget->getChildByTag(Tag::TAG_LABEL));
            CC_BREAK_IF(!label);
            if (code) {
                code->displayCode(label->getString().c_str());
            }
            break;
        }
        default:
            break;
    }
}




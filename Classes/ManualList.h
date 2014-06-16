//
//  ManualList.h
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#ifndef __CocosManual__ManualList__
#define __CocosManual__ManualList__

#include <iostream>
#include "GameInclude.h"

class ManualCode;

USING_NS_ALL

class ManualList : public Layer {
    
    
public:
    enum Tag {
        TAG_LISTVIEW    = 100,
        TAG_LABEL       = 101,
    };
    
    CREATE_FUNC(ManualList);
    virtual bool init();
    
    CC_SYNTHESIZE(ManualCode *,     code,       Code);
    
    void onTouchItem(Button * pSender, TouchEventType type);
    void onListviewEvent(ListView * pSender, ListViewEventType type);
    
};

#endif /* defined(__CocosManual__ManualList__) */

//
//  CCBLayer.h
//  CocosManual
//
//  Created by Jiangy on 14/6/4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __CocosManual__CCBLayer__
#define __CocosManual__CCBLayer__

#include <iostream>
#include "GameInclude.h"

USING_NS_ALL

class CCBLayer : CCB_LAYER_EXTEND {
private:
    // NOTE: declare private function and variable here
    bool loadCCBFinish;
    
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBLayer, create)
    DECLARE_CCNODE_FUNC(CCBLayer)
    DECLARE_STRUCTURE_FUNC(CCBLayer)
    DECLARE_CCB_LOADER_FUNC
    
    // NOTE: declare public function and variable(CC_SYNTHESIZE_RETAIN) here
    CC_SYNTHESIZE_RETAIN(Label *,   helloLabel,     HelloLabel);
    
protected:
    // NOTE: declare protected function and variable here
    void onPressButton(Ref * pSender);
    void onPressButton(Ref * pSender, Control::EventType type);
};

CREATE_CCB_LOADER_CLASS(CCBLayer, CCBLayerLoader)

#endif /* defined(__CocosManual__CCBLayer__) */
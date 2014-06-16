//
//  CCSLayer.h
//  CocosManual
//
//  Created by Jiangy on 14/6/12.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __CocosManual__CCSLayer__
#define __CocosManual__CCSLayer__

#include <iostream>
#include "GameInclude.h"

USING_NS_ALL

class CCSLayer : CCS_LAYER_EXTEND {
private:
    // NOTE: declare private function and variable here
    
public:
    DECLARE_CCNODE_FUNC(CCSLayer)
    CREATE_FUNC(CCSLayer)
    CCS_INIT_METHOD(CCSLayer)
    
    // NOTE: declare public function and variable(CC_SYNTHESIZE_RETAIN) here
    CC_SYNTHESIZE_RETAIN(Button *,          btnClick_0,         BtnClick_0);
    CC_SYNTHESIZE_RETAIN(Button *,          btnClick_1,         BtnClick_1);
    CC_SYNTHESIZE_RETAIN(Text *,            labHellos,          LabHellos);
    
    void onClickMe0(Ref * pSender, TouchEventType type);
    void onClickMe1(Ref * pSender, TouchEventType type);
    
protected:
    // NOTE: declare protected function and variable here
    
};

#endif /* defined(__CocosManual__CCSLayer__) */
//
//  ManualCode.h
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#ifndef __CocosManual__ManualCode__
#define __CocosManual__ManualCode__

#include <iostream>
#include "GameInclude.h"

class ManualDisplay;

USING_NS_ALL

class ManualCode : public Layer {
    
    
public:
    enum Tag {
        TAG_SCROLLVIEW  = 200,
        TAG_RET         = 201,
    };
    
    CREATE_FUNC(ManualCode);
    virtual bool init();
    
    CC_SYNTHESIZE(ManualDisplay *,          display,        Display);
    CC_SYNTHESIZE_RETAIN(ui::ScrollView *,  codeView,       CodeView);
    
    void displayCode(const char * key);
    
protected:
    void addCodeTexture(string text) const;
    
private:
    ValueMap mapCode;
    
};

#endif /* defined(__CocosManual__ManualCode__) */

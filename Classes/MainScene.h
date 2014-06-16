//
//  MainScene.h
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#ifndef __CocosManual__MainScene__
#define __CocosManual__MainScene__

#include <iostream>
#include "GameInclude.h"

USING_NS_ALL

class MainScene : public Layer {
    
    
public:
    enum Tag {
        TAG_CODELAYER   = 10,
    };
    
    CREATE_SCENE(MainScene);
    CREATE_FUNC(MainScene);
    
    virtual bool init();
    
    void onTouchTitle(Ref * pSender, TouchEventType type);
};


#endif /* defined(__CocosManual__MainScene__) */

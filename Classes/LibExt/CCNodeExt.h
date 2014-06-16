//
//  CCNodeExt.h
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#ifndef __CocosManual__CCNodeExt__
#define __CocosManual__CCNodeExt__

#include "cocos2d.h"

NS_CC_BEGIN

#pragma mark - NodeExt
class NodeExt : public Node {
    
};

#pragma mark - SceneExt
class SceneExt : public Scene {
    
};

#pragma mark - LayerExt
class LayerExt : public Layer {
    
};

#pragma mark - SpriteExt
class SpriteExt : public Sprite {
    
};

#pragma mark - LabelExt
class LabelExt : public Label {
    
};

#pragma mark - DrawNode
class DrawNodeExt : public DrawNode {
    
public:
    static DrawNodeExt * create();
    
    void drawRect(const Rect &rect, int stroke, const Color4F &borderColor);
    void fillRect(const Rect &rect, int stroke, const Color4F &borderColor, const Color4F &fillColor);
    
};


NS_CC_END

#endif /* defined(__CocosManual__CCNodeExt__) */

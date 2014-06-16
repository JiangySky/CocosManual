//
//  CCUtilsExt.h
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#ifndef __CocosManual__CCUtilsExt__
#define __CocosManual__CCUtilsExt__

#include <iostream>
#include "cocos2d.h"

NS_CC_BEGIN

class StringUtilsExt : public cocos2d::StringUtils {
    
public:
    static std::string stringFromPoint(const Point& pos);
    static std::string stringFromSize(const Size& size);
    static std::string stringFromRect(const Rect& rect);
};

NS_CC_END

#endif /* defined(__CocosManual__CCUtilsExt__) */

//
//  CCUtilsExt.cpp
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#include "CCUtilsExt.h"

NS_CC_BEGIN

std::string StringUtilsExt::stringFromPoint(const Point& pos)
{
    return StringUtils::format("{%.0f,%.0f}", pos.x, pos.y);
}

std::string StringUtilsExt::stringFromSize(const Size& size)
{
    return StringUtils::format("{%.0f,%.0f}", size.width, size.height);
}

std::string StringUtilsExt::stringFromRect(const Rect& rect)
{
    return StringUtils::format("{{%.0f,%.0f},{%.0f,%.0f}}", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}

NS_CC_END
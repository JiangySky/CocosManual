//
//  CCCommonExt.h
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#include <iostream>
#include "CCPlatformMacros.h"

enum E_NetworkStatus {
    kNotReachable = 0,
    kReachableViaWWAN,
    kReachableViaWiFi
};
typedef enum E_NetworkStatus NetworkStatus;

NS_CC_BEGIN

// cocos2d debug
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CCLOGFILE(_log)  do {} while (0)
#elif COCOS2D_DEBUG == 1
#define CCLOGFILE(_log)         cocos2d::LogFile(_log)
#elif COCOS2D_DEBUG > 1
#define CCLOGFILE(_log)         cocos2d::LogFile(_log)
#endif // COCOS2D_DEBUG

float CC_DLL LabelDimHeight(const char * content, const char * fontName, float fontSize, float width);
void CC_DLL LogFile(const std::string &log);
NetworkStatus CC_DLL NetworkEnable(const std::string &host = "");
void CC_DLL IgnorInteractionEvents(bool ignor, float delay = 0, bool clear = false);
const char * CC_DLL LocalizedString(const char * key);

NS_CC_END
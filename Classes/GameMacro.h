//
//  GameMacro.h
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#ifndef CocosManual_GameMacro_h
#define CocosManual_GameMacro_h

// MARK: quick getInstance
#define INSTANCE(__TYPE__)  __TYPE__::getInstance()

// MARK: namespace extension
#define USING_NA_ET     using namespace cocos2d::extension;

// MARK: namespace cocostudio
#define USING_NS_CS     using namespace cocostudio;

// MARK: namespace cocosbuilder
#define USING_NS_CCB    using namespace cocosbuilder;

// MARK: namespace ui
#define USING_NS_UI     using namespace cocos2d::ui;

// MARK: namespace network
#define USING_NS_NW     using namespace cocos2d::network;

// MARK: namespace all (std + cc + cs + ui)
#define USING_NS_ALL \
using namespace std; \
using namespace cocos2d; \
using namespace cocostudio; \
using namespace cocosbuilder; \
using namespace cocos2d::ui; \
using namespace cocos2d::extension; \
using namespace cocos2d::network;

#endif

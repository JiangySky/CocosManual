//
//  GameDefine.h
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#ifndef CocosManual_GameDefine_h
#define CocosManual_GameDefine_h

// MARK: cocos instance
#define APPLICATION                         Application::getInstance()
#define DIRECTOR                            Director::getInstance()
#define FILEUTILS                           FileUtils::getInstance()
#define AUDIO                               CocosDenshion::SimpleAudioEngine::getInstance()
#define USERDEFAULT                         UserDefault::getInstance()
#define EVENTDISPATCHER                     Director::getInstance()->getEventDispatcher()
#define SPRITEFRAMECACHE                    SpriteFrameCache::getInstance()
#define LOADPLIST(_pFile)                   SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_pFile)
#define TEXTURECACHE                        Director::getInstance()->getTextureCache()

#define RUNNINGSCENE                        Director::getInstance()->getRunningScene()
#define MAINLAYER                           Director::getInstance()->getRunningScene()->getChildAtIndex(0)

// MARK: createScene
#define CREATE_SCENE(__TYPE__) \
static cocos2d::Scene * createScene() \
{ \
    auto scene = cocos2d::Scene::create(); \
    auto layer = __TYPE__::create(); \
    scene->addChild(layer); \
    return scene; \
}

// MARK: bind widget and listener
#define BIND_WIDGET(_parent, _name, _type, _widget)    _widget = (_type)_parent->getChildByName(_name)

#define BIND_LISTENER_TOUCH(_widget, _target, _listener) \
if (_target && _widget) { \
    _widget->addTouchEventListener(_target, toucheventselector(_listener)); \
}
#define BIND_LISTENER_SV(_widget, _target, _listener) \
if (_target && _widget) { \
    _widget->addEventListenerScrollView(_target, scrollvieweventselector(_listener)); \
}
#define BIND_LISTENER_LV(_widget, _target, _listener) \
if (_target && _widget) { \
_widget->addEventListenerListView(_target, listvieweventselector(_listener)); \
}
#define BIND_LISTENER_PV(_widget, _target, _listener) \
if (_target && _widget) { \
_widget->addEventListenerPageView(_target, pagevieweventselector(_listener)); \
}
#define BIND_LISTENER_CB(_widget, _target, _listener) \
if (_target && _widget) { \
    _widget->addEventListenerCheckBox(_target, checkboxselectedeventselector(_listener)); \
}
#define BIND_LISTENER_SL(_widget, _target, _listener) \
if (_target && _widget) { \
    _widget->addEventListenerSlider(_target, sliderpercentchangedselector(_listener)); \
}

#define BIND_WIDGET_WITH_LISTENER_TOUCH(_parent, _name, _type, _widget, _target, _listener) \
BIND_WIDGET(_parent, _name, _type, _widget); \
BIND_LISTENER_TOUCH(_widget, _target, _listener)

// MARK: Widget enabel
#define WIDGET_ENABEL(_widget, _enabel) \
_widget->setTouchEnabled(_enabel); \
_widget->setBright(_enabel);

// MARK: equal
#define STRING_IS_EQUAL(_a, _b)     strcmp(_a, _b) == 0

// MARK: Node center
#define NODE_CENTER(_node)          Point::ZERO.getMidpoint(Point(_node->getContentSize()))

// MARK: void * <--> string
#define STRING2VPTR(_s)             static_cast<void *>(new std::string(_s))
#define VPTR2STRING(_v)             *(static_cast<std::string *>(_v))

// MARK: math logic -
#define FLOAT_TOLERATE                  0.001
#define FLOAT_EQUAL(x, y)               ((x) - (y) < FLOAT_TOLERATE && (x) - (y) > -FLOAT_TOLERATE)
#define FLOAT_NOT_EQUAL(x, y)           ((x) - (y) > FLOAT_TOLERATE || (x) - (y) < -FLOAT_TOLERATE)
#define POSITION_EQUAL(x, y)            ((x) - (y) < 1 && (x) - (y) > -1)
#define POSITION_NOT_EQUAL(x, y)        ((x) - (y) > 1 || (x) - (y) < -1)

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS(__ANGLE__)   ((__ANGLE__) / 180.0 * M_PI)
#define RADIANS_TO_DEGREES(__ANGLE__)   ((__ANGLE__) / M_PI * 180.0)
#endif

#ifndef SWAP
#define SWAP(_a, _b)                    __typeof__(_a) temp; temp = _a; _a = _b; _b = temp
#define CLAMP(_value, _lower, _upper)   (_value = MIN(MAX(_lower, _value), _upper))
#endif

#ifndef VALUE_BETWEEN_OO
#define VALUE_BETWEEN_OO(_value, _lower, _upper)    (_value > _lower && _value < _upper)
#define VALUE_BETWEEN_OC(_value, _lower, _upper)    (_value > _lower && _value <= _upper)
#define VALUE_BETWEEN_CO(_value, _lower, _upper)    (_value >= _lower && _value < _upper)
#define VALUE_BETWEEN_CC(_value, _lower, _upper)    (_value >= _lower && _value <= _upper)
#endif

#ifndef RANDOM_CO
#define RANDOM_CO(_from, _to)       random(_from, _to)
#define RANDOM_CC(_from, _to)       random(_from, (_to + 1))
#endif

// MARK: CocosBuilder -
// MARK: create CCB Layer header
#define CCB_LAYER_EXTEND    \
public cocos2d::Layer,  \
public cocosbuilder::CCBSelectorResolver, \
public cocosbuilder::CCBMemberVariableAssigner, \
public cocosbuilder::NodeLoaderListener

// MARK: create CCB Node header
#define CCB_NODE_EXTEND    \
public cocos2d::Node,  \
public cocosbuilder::CCBSelectorResolver, \
public cocosbuilder::CCBMemberVariableAssigner, \
public cocosbuilder::NodeLoaderListener

// MARK: declare COCOS2D structure function
#define DECLARE_CCNODE_FUNC(_className) \
static cocos2d::Scene * scene(); \
static _className * layer(); \
bool addWithScene;

// MARK: declare structure function
#define DECLARE_STRUCTURE_FUNC(_className) \
_className(); \
virtual ~_className(); \
virtual bool init();

// MARK: declare CCB load function
#define DECLARE_CCB_LOADER_FUNC     \
virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char * pSelectorName); \
virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char * pSelectorName); \
virtual bool onAssignCCBMemberVariable(Ref * pTarget, const char* pMemberVariableName, Node * pNode); \
virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader); \
virtual void didLoadFromCCB();

// MARK: create CCBLoader
#define CREATE_CCB_LOADER_CLASS(_className, _classLoaderName) \
class _classLoaderName : public cocosbuilder::LayerLoader { \
public: \
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(_classLoaderName, loader); \
protected: \
CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(_className); \
};

// MARK: create CCB class
#define CREATE_CCB_SINGLE_CLASS(_className, _classLoadName) \
class _className : CCB_LAYER_EXTEND { \
public: \
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(_className, create) \
DECLARE_CCNODE_FUNC(_className) \
DECLARE_STRUCTURE_FUNC(_className) \
DECLARE_CCB_LOADER_FUNC \
}; \
CREATE_CCB_LOADER_CLASS(_className, _classLoadName)

// MARK: regist ccbNoderLoader
#define REGIST_CCB_LOADER(_className, _classLoadName) \
cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader(_className, _classLoadName::loader());

// MARK: create ccbReader
#define CREATE_CCB_READER(_className, _classLoadName) \
cocosbuilder::NodeLoaderLibrary * nodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::getInstance(); \
nodeLoaderLibrary->registerNodeLoader(_className, _classLoadName::loader()); \
cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(nodeLoaderLibrary); \
ccbReader->autorelease();

// MARK: implement CCBScene
#define SCENE_FROM_CCB(_className, _classLoadName, _ccbName, _ccbRefName) \
cocos2d::Scene * _className::scene() { \
CREATE_CCB_READER(_ccbRefName, _classLoadName); \
_className * layer = (_className *)ccbReader->readNodeGraphFromFile(_ccbName); \
if (layer) { \
layer->addWithScene = true; \
layer->setKeypadEnabled(true); \
Scene * scene = Scene::create(); \
scene->addChild(layer); \
return scene; \
} \
return nullptr; \
}

// MARK: implement CCBLayer
#define LAYER_FROM_CCB(_className, _classLoadName, _ccbName, _ccbRefName) \
_className * _className::layer() { \
CREATE_CCB_READER(_ccbRefName, _classLoadName); \
_className * layer = (_className *)ccbReader->readNodeGraphFromFile(_ccbName); \
if (layer) { \
layer->addWithScene = false; \
} \
return layer; \
}

#define CREATER_FROM_CCB(_className, _classLoadName, _ccbName, _ccbRefName) \
SCENE_FROM_CCB(_className, _classLoadName, _ccbName, _ccbRefName) \
LAYER_FROM_CCB(_className, _classLoadName, _ccbName, _ccbRefName)

// MARK: create CCBNode instance
#define NODE_FROM_CCB(_node, _type, _ccbName, _owner, _ownerClassName, _classLoadName) \
CREATE_CCB_READER(_ownerClassName, _classLoadName); \
_node = (_type *)ccbReader->readNodeGraphFromFile(_ccbName, _owner);


// MARK: CocoStudio -
#define CCS_FILE_TAG    -999
// MARK: create CCS Layer header
#define CCS_LAYER_EXTEND        public cocos2d::Layer
#define CCS_INIT_METHOD(_class) \
~_class(); \
void bindFromCCS(); \
void onBindFinish(); \
void reset();

// MARK: implement CCSScene
#define SCENE_FROM_CCS(_class, _ccsName) \
cocos2d::Scene * _class::scene() \
{ \
cocos2d::Scene * scene = cocos2d::Scene::create(); \
_class * layer = _class::create(); \
auto uiLayout = INSTANCE(cocostudio::GUIReader)->widgetFromJsonFile(_ccsName); \
if (layer && uiLayout) { \
layer->addChild(uiLayout, 0, CCS_FILE_TAG); \
layer->bindFromCCS(); \
layer->addWithScene = true; \
scene->addChild(layer); \
} \
return scene; \
}

// MARK: implement CCSLayer
#define LAYER_FROM_CCS(_class, _ccsName) \
_class * _class::layer() { \
_class * layer = _class::create(); \
auto uiLayout = INSTANCE(cocostudio::GUIReader)->widgetFromJsonFile(_ccsName); \
if (layer && uiLayout) { \
layer->addChild(uiLayout, 0, CCS_FILE_TAG); \
layer->bindFromCCS(); \
layer->addWithScene = false; \
} \
return layer; \
}

#define CREATER_FROM_CCS(_class, _ccsName) \
SCENE_FROM_CCS(_class, _ccsName) \
LAYER_FROM_CCS(_class, _ccsName)


#endif

//
//  ManualDisplay.cpp
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#include "ManualDisplay.h"
#include "CCBLayer.h"
#include "CCSLayer.h"
#include "Connect/AppConnect.h"

static int tick = 0;
static int currFrame = 0;

bool ManualDisplay::init()
{
    Size sizeWin = DIRECTOR->getWinSize();
    float listWidth = sizeWin.width - sizeWin.height + 5;
    ignoreContentAdaptWithSize(false);
    setSize(Size(sizeWin.width - listWidth, sizeWin.height * 2 / 3 - 5));
    setPosition(Point(listWidth, sizeWin.height / 3 + 5));
    setClippingEnabled(true);
    setClippingType(LAYOUT_CLIPPING_STENCIL);
    
    runCode("Welcome");
    
    return true;
}

void ManualDisplay::update(float time)
{
    switch (manualType) {
        case ManualType::LAYERCOLOR: {
            auto layerColor = getChildByTag(Tag::TAG_LAYERCOLOR);
            CC_BREAK_IF(!layerColor);
            auto color = layerColor->getColor();
            if (color.r < 255) {
                if (++color.r == 255) {
                    color.g = 0;
                }
            }
            else if (color.g < 255) {
                if (++color.g == 255) {
                    color.b = 0;
                }
            }
            else if (color.b < 255) {
                if (++color.b == 255) {
                    color.r = 0;
                }
            }
            layerColor->setColor(color);
            break;
        }
        case ManualType::LAYERGRADIENT: {
            static int style = 0;
            LayerGradient * layerGradient = dynamic_cast<LayerGradient *>(getChildByTag(Tag::TAG_LAYERGRADIENT));
            CC_BREAK_IF(!layerGradient);
            Point alongPos = layerGradient->getVector();
            switch (style) {
                case 0:
                    alongPos += Point(1, 1);
                    if (alongPos == Point(layerGradient->getContentSize())) {
                        alongPos.y = 0;
                        style++;
                    }
                    break;
                    
                case 1:
                    alongPos += Point(-1, 1);
                    if (alongPos == Point(0, layerGradient->getContentSize().height)) {
                        alongPos = alongPos.getMidpoint(Point::ZERO);
                        style++;
                    }
                    break;
                    
                case 2:
                    alongPos += Point(1, 0);
                    if (alongPos.x == layerGradient->getContentSize().width) {
                        alongPos = Point(layerGradient->getContentSize().width / 2, 0);
                        style++;
                    }
                    break;
                    
                case 3:
                    alongPos += Point(0, 1);
                    if (alongPos.y == layerGradient->getContentSize().height) {
                        alongPos = Point::ZERO;
                        style++;
                    }
                    break;
                    
                case 4:
                    alongPos += Point(1, 0);
                    if (alongPos.x == layerGradient->getContentSize().width) {
                        style++;
                    }
                    break;
                    
                case 5:
                    alongPos += Point(0, 1);
                    if (alongPos.y == layerGradient->getContentSize().height) {
                        style++;
                    }
                    break;
                    
                case 6:
                    alongPos += Point(-1, 0);
                    if (alongPos.x == 0) {
                        style++;
                    }
                    break;
                    
                case 7:
                    alongPos += Point(0, -1);
                    if (alongPos.y == 0) {
                        style = 0;
                    }
                    break;
                    
                default:
                    break;
            }
            
            layerGradient->setVector(alongPos);
            
            DrawNode * drawNode = dynamic_cast<DrawNode *>(layerGradient->getChildByTag(Tag::TAG_DRAWNODE));
            CC_BREAK_IF(!drawNode);
            drawNode->clear();
            drawNode->drawDot(alongPos, 2, Color4F::BLACK);
            break;
        }
        case ManualType::SPRITE: {
            CC_BREAK_IF(++tick / 10 == currFrame);
            
            auto nodeSpr = getChildByTag(Tag::TAG_NODESPRITE);
            CC_BREAK_IF(!nodeSpr);
            
            Sprite * sprFrame = dynamic_cast<Sprite *>(nodeSpr->getChildByTag(Tag::TAG_SPRITEFRAME));
            CC_BREAK_IF(!sprFrame);
            
            currFrame = tick / 10;
            if (currFrame > 7) {
                currFrame = 0;
                tick = 0;
            }

            string key = StringUtils::format("10003_move_000%i.png", currFrame);
            sprFrame->setSpriteFrame(key.c_str());
            break;
        }
        case ManualType::UIIMAGEVIEW: {
            CC_BREAK_IF(++tick / 10 == currFrame);
            
            auto nodeImv = getChildByTag(Tag::TAG_NODESPRITE);
            CC_BREAK_IF(!nodeImv);
            
            ImageView * imvFrame = dynamic_cast<ImageView *>(nodeImv->getChildByTag(Tag::TAG_IMAGEVIEW));
            CC_BREAK_IF(!imvFrame);
            
            currFrame = tick / 10;
            if (currFrame > 7) {
                currFrame = 0;
                tick = 0;
            }
            
            string key = StringUtils::format("10003_move_000%i.png", currFrame);
            imvFrame->loadTexture(key, UI_TEX_TYPE_PLIST);
            break;
        }
        case ManualType::UILOADING: {
            if (++tick > 100) {
                tick = 0;
            }
            
            auto ldbLeft = getChildByTag(Tag::TAG_LOADINGLEFT);
            CC_BREAK_IF(!ldbLeft);
            static_cast<LoadingBar *>(ldbLeft)->setPercent(tick);
            
            auto * ldbRight = getChildByTag(Tag::TAG_LOADINGRIGHT);
            CC_BREAK_IF(!ldbRight);
            static_cast<LoadingBar *>(ldbRight)->setPercent(tick);
            
            auto * ldbExtL = getChildByTag(Tag::TAG_LOADINGEXTL);
            CC_BREAK_IF(!ldbExtL);
            static_cast<LoadingBarExt *>(ldbExtL)->setPercent(tick);
            
            auto * ldbExtR = getChildByTag(Tag::TAG_LOADINGEXTR);
            CC_BREAK_IF(!ldbExtR);
            static_cast<LoadingBarExt *>(ldbExtR)->setPercent(tick);
            
            auto * ldbExtLL = getChildByTag(Tag::TAG_LOADINGEXTLL);
            CC_BREAK_IF(!ldbExtLL);
            static_cast<LoadingBarExt *>(ldbExtLL)->setPercent(tick);
            
            if (++currFrame > 230) {
                currFrame = 0;
            }
            auto * ldbExt = getChildByTag(Tag::TAG_LOADINGEXT);
            CC_BREAK_IF(!ldbExt);
            static_cast<LoadingBarExt *>(ldbExt)->setPercent(currFrame / 2.3);
            break;
        }
        case ManualType::CONNECT: {
            if (currFrame > 0) {
                CC_BREAK_IF(++tick % 30 != 0);
                AppConnect::server()->sendHeartBeat(nullptr);
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::setManualType(ManualDisplay::ManualType type)
{
    currFrame = 0;
    tick = 0;
    manualType = type;
    if (manualType == ManualType::LAYERCOLOR
        || manualType == ManualType::LAYERGRADIENT
        || manualType == ManualType::SPRITE
        || manualType == ManualType::UIIMAGEVIEW
        || manualType == ManualType::UILOADING
        || manualType == ManualType::CONNECT) {
        scheduleUpdate();
    }
}

void ManualDisplay::runCode(const char *key)
{
    resetDisplay();
    
    if (STRING_IS_EQUAL(key, "Welcome")
        || STRING_IS_EQUAL(key, "Scene")
        || STRING_IS_EQUAL(key, "Layer")) {
        displayWelcome(key);
    }
    else if (STRING_IS_EQUAL(key, "LayerColor")) {
        setManualType(ManualType::LAYERCOLOR);
        displayLayerColor();
    }
    else if (STRING_IS_EQUAL(key, "LayerGradient")) {
        setManualType(ManualType::LAYERGRADIENT);
        displayLayerGradient();
    }
    else if (STRING_IS_EQUAL(key, "LayerMultiplex")) {
        displayLayerMultiplex();
    }
    else if (STRING_IS_EQUAL(key, "Sprite")) {
        setManualType(ManualType::SPRITE);
        displaySprite();
    }
    else if (STRING_IS_EQUAL(key, "Label")) {
        displayLabel();
    }
    else if (STRING_IS_EQUAL(key, "Menu")) {
        displayMenu();
    }
    else if (STRING_IS_EQUAL(key, "UI-Layout")) {
        displayUILayout();
    }
    else if (STRING_IS_EQUAL(key, "UI-Text")) {
        displayUIText();
    }
    else if (STRING_IS_EQUAL(key, "UI-ImageView")) {
        setManualType(ManualType::UIIMAGEVIEW);
        displayUIImageView();
    }
    else if (STRING_IS_EQUAL(key, "UI-Button")) {
        displayUIButton();
    }
    else if (STRING_IS_EQUAL(key, "UI-LoadingBar")) {
        setManualType(ManualType::UILOADING);
        displayUILoadingBar();
    }
    else if (STRING_IS_EQUAL(key, "UI-CheckBox")) {
        displayUICheckBox();
    }
    else if (STRING_IS_EQUAL(key, "UI-Slide")) {
        displayUISlide();
    }
    else if (STRING_IS_EQUAL(key, "UI-ScrollView")) {
        displayUIScrollView();
    }
    else if (STRING_IS_EQUAL(key, "UI-ListView")) {
        displayUIListView();
    }
    else if (STRING_IS_EQUAL(key, "UI-PageView")) {
        displayUIPageView();
    }
    else if (STRING_IS_EQUAL(key, "UI-Box")) {
        displayUIBox();
    }
    else if (STRING_IS_EQUAL(key, "CocosBuilder")) {
        displayCCBuilder();
    }
    else if (STRING_IS_EQUAL(key, "CocoStudio")) {
        displayCCStudio();
    }
    else if (STRING_IS_EQUAL(key, "TileMap")) {
        displayTileMap();
    }
    else if (STRING_IS_EQUAL(key, "Particle")) {
        displayParticle();
    }
    else if (STRING_IS_EQUAL(key, "Action")) {
        displayAction();
    }
    else if (STRING_IS_EQUAL(key, "Connect")) {
        setManualType(ManualType::CONNECT);
        displayConnect();
    }
    
}

#pragma mark -

void ManualDisplay::resetDisplay()
{
    tick = 0;
    unscheduleUpdate();
    setManualType(ManualType::NONE);
    removeAllChildrenWithCleanup(true);
}

void ManualDisplay::displayWelcome(const char *key)
{
    Point pos = Point(getContentSize());
    Point posLabel = pos.getMidpoint(Point::ZERO);
    auto label = Label::createWithSystemFont("Cocos2dx 3.0 Manual", "Marker Felt", 50);
    label->setColor(Color3B::GREEN);
    label->enableShadow();
    label->setPosition(posLabel);
    addChild(label);
    
    if (strcmp(key, "Welcome") != 0) {
        string tip = StringUtils::format("Ignore display %s", key);
        auto info = Label::createWithSystemFont(tip.c_str(), "Helvetica", 30);
        info->setColor(Color3B::RED);
        info->enableOutline(Color4B::BLUE, 3);
        info->setPosition(posLabel - Point(0, 50));
        addChild(info);
    }
}

void ManualDisplay::displayScene()
{
    auto scene = Scene::create();
    DIRECTOR->replaceScene(scene);
}

void ManualDisplay::displayLayer()
{
    auto layer = Layer::create();
    addChild(layer);
}

void ManualDisplay::displayLayerColor()
{
    float width = 200;
    
    // red layer
    // (not ignore anchor for position)
    auto layerRed = LayerColor::create(Color4B::RED, width, width);
    layerRed->setPosition(Point(width / 2, width / 2));
    layerRed->ignoreAnchorPointForPosition(false);
    addChild(layerRed);
    
    // blue layer with translucent
    // (ignore anchor for position [default])
    auto layerBlue = LayerColor::create(Color4B(0, 0, 255, 100), width, width);
    layerBlue->setPosition(Point(width / 2, width / 2));
    addChild(layerBlue);
    
    // layer update color
    Point pos = Point(getContentSize());
    auto layerColor = LayerColor::create(Color4B::BLACK);
    layerColor->setTag(Tag::TAG_LAYERCOLOR);
    layerColor->setContentSize(Size(width * 2, width * 2));
    layerColor->ignoreAnchorPointForPosition(false);
    layerColor->setPosition(pos - Point(width + 5, width + 5));
    addChild(layerColor);
    
    // !!!: update color in update()
}

void ManualDisplay::displayLayerGradient()
{
    float width = 200;
    // red layer
    // (not ignore anchor for position)
    auto layerRed = LayerGradient::create(Color4B::RED, Color4B::YELLOW);
    layerRed->setContentSize(Size(width, width));
    layerRed->setPosition(Point(width / 2, width / 2));
    layerRed->ignoreAnchorPointForPosition(false);
    addChild(layerRed);
    
    // blue layer with translucent
    // (ignore anchor for position [default])
    auto layerBlue = LayerGradient::create(Color4B::BLUE, Color4B::GREEN);
    layerBlue->setOpacity(100);
    layerBlue->setContentSize(Size(width, width));
    layerBlue->setPosition(Point(width / 2, width / 2));
    addChild(layerBlue);
    
    // layer update along point
    Point pos = Point(getContentSize());
    auto layerGradient = LayerGradient::create(Color4B::MAGENTA, Color4B::ORANGE, Point::ZERO);
    layerGradient->setTag(Tag::TAG_LAYERGRADIENT);
    layerGradient->setContentSize(Size(width * 2, width * 2));
    layerGradient->ignoreAnchorPointForPosition(false);
    layerGradient->setPosition(pos - Point(width + 5, width + 5));

    auto drawNode = DrawNode::create();
    drawNode->setTag(Tag::TAG_DRAWNODE);
    drawNode->setPosition(Point::ZERO);
    layerGradient->addChild(drawNode);
    
    addChild(layerGradient);
    
    // !!!: update along point in update()
}

void ManualDisplay::displayLayerMultiplex()
{
    float width = 400;
    const int Amount = 6;
    Point posBtn = Point(getContentSize()) - Point(55, 55);
    auto layerMultiplex = LayerMultiplex::create();
    Color4B color[Amount] = {Color4B::RED, Color4B::GREEN, Color4B::BLUE, Color4B::ORANGE, Color4B::YELLOW, Color4B::MAGENTA};
    for (int i = 0; i < Amount; i++) {
        auto layerColor = LayerColor::create(color[i], width, width);
        layerColor->ignoreAnchorPointForPosition(false);
        layerMultiplex->addLayer(layerColor);
        
        auto btn = Button::create("white.png");
        BIND_LISTENER_TOUCH(btn, this, ManualDisplay::onSwitchLayer);
        btn->setColor(Color3B(color[i]));
        btn->setTag(i);
        btn->setPosition(posBtn);
        posBtn.y -= 80;
        addChild(btn);
    }
    layerMultiplex->setPosition(NODE_CENTER(this));
    layerMultiplex->switchTo(0);
    layerMultiplex->setTag(Tag::TAG_LAYERMULTIPLEX);
    addChild(layerMultiplex);
    
    // !!!: switch layer in onSwitchLayer()
}

void ManualDisplay::displaySprite()
{
    auto nodeSprites = Node::create();
    Point posCenter = NODE_CENTER(this);
    Size size;
    Point pos;
    Rect rect;
    
    // sprite from png (The whole of dragon)
    auto sprPng = Sprite::create("dragon.png");
    sprPng->setPosition(posCenter - Point(80, 30));
    nodeSprites->addChild(sprPng);
    
    // sprite from png with rect (The part of dragon)
    size = sprPng->getContentSize();
    pos = sprPng->getPosition();
    size.width /= 2;
    pos.x += size.width;
    rect = Rect(size.width, 0, size.width, size.height);
    auto sprPngRect = Sprite::create("dragon.png", rect);
    sprPngRect->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    sprPngRect->setPosition(pos);
    nodeSprites->addChild(sprPngRect);
    
    // load plist file
    LOADPLIST("10003_stand.plist");
    LOADPLIST("10003_move.plist");
    Point posFrame = Point(100, 400);
    
    // sprite from frame (The people standing)
    auto frame = SPRITEFRAMECACHE->getSpriteFrameByName("10003_stand_0000.png");
    auto sprFrame = Sprite::createWithSpriteFrame(frame);
    sprFrame->setPosition(posFrame);
    nodeSprites->addChild(sprFrame);
    
    // sprite from frame name (The people running)
    auto sprFrameName = Sprite::createWithSpriteFrameName("10003_move_0000.png");
    sprFrameName->setPosition(posFrame + Point(140, 0));
    sprFrameName->setTag(Tag::TAG_SPRITEFRAME);
    nodeSprites->addChild(sprFrameName);
    
    // sprite frome texture (The whole of wolf)
    auto texture = TEXTURECACHE->addImage("wolf.png");
    auto sprTexture = Sprite::createWithTexture(texture);
    sprTexture->setPosition(posFrame + Point(400, 50));
    nodeSprites->addChild(sprTexture);
    
    // sprite from texture with rect (The part of wolf)
    size = sprTexture->getContentSize();
    pos = sprTexture->getPosition();
    size.width /= 2;
    pos.x += size.width;
    rect = Rect(size.width, 0, size.width, size.height);
    auto sprTextRect = Sprite::createWithTexture(texture, rect);
    sprTextRect->setPosition(pos);
    nodeSprites->addChild(sprTextRect);
    
    nodeSprites->setTag(Tag::TAG_NODESPRITE);
    addChild(nodeSprites);
    
    // !!!: update spriteFrame(runner) in update
    // !!!: setSpriteFrame, not runAction
    
    // add sprite controller
    addSpriteController();
    
    // !!!: change sprite attribute by controller
}

void ManualDisplay::displayLabel()
{
    float posY = getContentSize().height - 50;
    Point pos = Point(150, posY);
    
    for (int i = 0; i < 4; i++) {
        // label with system font
        auto labSys = Label::createWithSystemFont("SystemFont", "Marker Felt", 30);
        if (labSys) {
            labSys->setPosition(pos);
            labSys->setColor(Color3B::MAGENTA);
            switch (i) {
                case 0: // no effect (col 1)
                    labSys->disableEffect();
                    break;
                    
                case 1: // shadow (col 2)
                    labSys->enableShadow();
                    break;
                
                case 2: // outline (col 3)
                    labSys->enableOutline(Color4B::GREEN, 3);
                    break;
                
                case 3: // grow (col 4)
                    labSys->enableGlow(Color4B::BLUE);
                    break;
                    
                default:
                    break;
            }
            addChild(labSys);
        }
        
        pos.y -= 100;
        
        // label with ttf
        auto labTTF = Label::createWithTTF("LabelTTF", "pacifico.ttf", 30);
        if (labTTF) {
            labTTF->setPosition(pos);
            labTTF->setColor(Color3B::ORANGE);
            switch (i) {
                case 0: // no effect (col 1)
                    labTTF->disableEffect();
                    break;
                    
                case 1: // shadow (col 2)
                    labTTF->enableShadow();
                    break;
                    
                case 2: // outline (col 3)
                    labTTF->enableOutline(Color4B::GREEN, 3);
                    break;
                    
                case 3: // grow (col 4)
                    labTTF->enableGlow(Color4B::BLUE);
                    break;
                    
                default:
                    break;
            }
            addChild(labTTF);
        }
        
        pos.y -= 100;
        
        // label with BMFont (520)
        auto labBMF = Label::createWithBMFont("font_vip.fnt", "520");
        if (labBMF) {
            labBMF->setPosition(pos);
            switch (i) {
                case 0: // no effect (col 1)
                    labBMF->disableEffect();
                    break;
                    
                case 1: // shadow (col 2)
                    labBMF->enableShadow();
                    break;
                    
                case 2: // outline (col 3)
                    labBMF->enableOutline(Color4B::GREEN, 3);
                    break;
                    
                case 3: // grow (col 4)
                    labBMF->enableGlow(Color4B::BLUE);
                    break;
                    
                default:
                    break;
            }
            addChild(labBMF);
        }
        
        pos.y -= 100;
        
        // label with CharMap (1314)
        auto labCMap = Label::createWithCharMap("font_power.png", 27, 28, '0');
        if (labCMap) {
            labCMap->setPosition(pos);
            labCMap->setString("1314");
            switch (i) {
                case 0: // no effect (col 1)
                    labCMap->disableEffect();
                    break;
                    
                case 1: // shadow (col 2)
                    labCMap->enableShadow();
                    break;
                    
                case 2: // outline (col 3)
                    labCMap->enableOutline(Color4B::GREEN, 3);
                    break;
                    
                case 3: // grow (col 4)
                    labCMap->enableGlow(Color4B::BLUE);
                    break;
                    
                default:
                    break;
            }
            addChild(labCMap);
        }
        
        pos.x += 150;
        posY -= 40;
        pos.y = posY;
    }
}

void ManualDisplay::displayMenu()
{
    const int MenuAmount = 6;
    const string Menu[] = {"NewGame", "Continue", "Highscore", "Help", "About", "MoreGame"};
    
    // add title
    auto labTitle = Label::createWithSystemFont("MentTitle", "Marker Felt", 40);
    labTitle->setColor(Color3B::RED);
    labTitle->enableShadow();
    labTitle->setPosition(Point(getContentSize().width / 2, getContentSize().height - 60));
    labTitle->setTag(Tag::TAG_MENUTITLE);
    addChild(labTitle);
    
    // add menu with label (center)
    Vector<MenuItem *> vecItem;
    for (int i = 0; i < MenuAmount; i++) {
        auto label = Label::createWithTTF(Menu[i].c_str(), "pacifico.ttf", 30);
        label->setColor(Color3B::GREEN);
        label->enableShadow();
        auto menuItem = MenuItemLabel::create(label, [&](Ref * pSender) {
            MenuItem * menuItem = dynamic_cast<MenuItem *>(pSender);
            int tag = menuItem->getTag();
            if (tag == 0) {
                auto miCont = menuItem->getParent()->getChildByTag(1);
                dynamic_cast<MenuItem *>(miCont)->setEnabled(true);
            }
            auto menuTitle = this->getChildByTag(Tag::TAG_MENUTITLE);
            if (menuTitle) {
                string label = VPTR2STRING(menuItem->getUserData());
                dynamic_cast<Label *>(menuTitle)->setString(label.c_str());
            }
        });
        menuItem->setDisabledColor(Color3B::GRAY);
        menuItem->setTag(i);
        menuItem->setUserData(STRING2VPTR(Menu[i]));
        if (i == 1) {
            menuItem->setEnabled(false);
        }
        vecItem.pushBack(menuItem);
    }
    auto menuLabel = Menu::createWithArray(vecItem);
    menuLabel->alignItemsVerticallyWithPadding(5);
    menuLabel->setPosition(NODE_CENTER(this) - Point(0, 40));
    addChild(menuLabel);
    
    vecItem.clear();
    LOADPLIST("menu.plist");
    
    // add menu with image [sprite] (left)
    for (int i = 0; i < MenuAmount; i++) {
        auto menuItem = MenuItemImage::create();
        string frmNormal = StringUtils::format("menu_%i_%s.png", i, "n");
        string frmSelect = StringUtils::format("menu_%i_%s.png", i, "s");
        string frmDisable = StringUtils::format("menu_%i_%s.png", i, "d");
        menuItem->setNormalSpriteFrame(SPRITEFRAMECACHE->getSpriteFrameByName(frmNormal.c_str()));
        menuItem->setSelectedSpriteFrame(SPRITEFRAMECACHE->getSpriteFrameByName(frmSelect.c_str()));
        menuItem->setDisabledSpriteFrame(SPRITEFRAMECACHE->getSpriteFrameByName(frmDisable.c_str()));
        menuItem->setCallback([&](Ref * pSender) {
            MenuItem * menuItem = dynamic_cast<MenuItem *>(pSender);
            int tag = menuItem->getTag();
            if (tag == 0) {
                auto miCont = menuItem->getParent()->getChildByTag(1);
                dynamic_cast<MenuItem *>(miCont)->setEnabled(true);
            }
            auto menuTitle = this->getChildByTag(Tag::TAG_MENUTITLE);
            if (menuTitle) {
                string label = VPTR2STRING(menuItem->getUserData());
                dynamic_cast<Label *>(menuTitle)->setString(label.c_str());
            }
        });
        menuItem->setTag(i);
        menuItem->setUserData(STRING2VPTR(Menu[i]));
        if (i == 1) {
            menuItem->setEnabled(false);
        }
        vecItem.pushBack(menuItem);
    }
    auto menuImage = Menu::createWithArray(vecItem);
    menuImage->alignItemsVerticallyWithPadding(5);
    menuImage->setPosition(NODE_CENTER(this) - Point(220, 40));
    addChild(menuImage);
    
    vecItem.clear();
    
    // add menu with toggle (right)
    for (int i = 0; i < MenuAmount; i++) {
        string frmNormal = StringUtils::format("menu_%i_%s.png", i, "n");
        string frmSelect = StringUtils::format("menu_%i_%s.png", i, "s");
        auto sprNormal = Sprite::createWithSpriteFrameName(frmNormal.c_str());
        auto sprSelect = Sprite::createWithSpriteFrameName(frmSelect.c_str());
        auto menuItem = MenuItemSprite::create(sprNormal, sprSelect);
        menuItem->setTag(i);
        menuItem->setUserData(STRING2VPTR(Menu[i]));
        if (i == 1) {
            menuItem->setEnabled(false);
        }
        vecItem.pushBack(menuItem);
    }
    auto menuItemToggle = MenuItemToggle::createWithCallback([&](Ref * pSender) {
        MenuItemToggle * menuItemToggle = dynamic_cast<MenuItemToggle *>(pSender);
        MenuItem * menuItem = menuItemToggle->getSelectedItem();
        int tag = menuItem->getTag();
        if (tag == 0) {
            auto miCont = menuItem->getParent()->getChildByTag(1);
            dynamic_cast<MenuItem *>(miCont)->setEnabled(true);
        }
        auto menuTitle = this->getChildByTag(Tag::TAG_MENUTITLE);
        if (menuTitle) {
            string label = VPTR2STRING(menuItem->getUserData());
            dynamic_cast<Label *>(menuTitle)->setString(label.c_str());
        }
    }, vecItem);
    auto menuToggle = Menu::createWithItem(menuItemToggle);
    menuToggle->alignItemsVerticallyWithPadding(5);
    menuToggle->setPosition(NODE_CENTER(this) - Point(-220, 40));
    addChild(menuToggle);
}

void ManualDisplay::displayUILayout()
{
    float width = 200;
    
    // red layout (ANCHOR_MIDDLE)
    auto latRed = Layout::create();
    latRed->setSize(Size(200, 200));
    latRed->setBackGroundColor(Color3B::RED);
    latRed->setBackGroundColorType(LAYOUT_COLOR_SOLID);
    latRed->setAnchorPoint(Point::ANCHOR_MIDDLE);
    latRed->setPosition(Point(width / 2, width / 2));
    addChild(latRed);
    
    // gradient layout (ANCHOR_ZERO)
    auto latGrd = Layout::create();
    latGrd->setSize(Size(200, 200));
    latGrd->setBackGroundColor(Color3B::BLUE, Color3B::YELLOW);
    latGrd->setBackGroundColorType(LAYOUT_COLOR_GRADIENT);
    latGrd->setPosition(Point(width, width));
    addChild(latGrd);
    
    // none bg layout
    auto latClear = Layout::create();
    latGrd->setSize(Size(200, 200));
    auto drawNode = DrawNode::create();
    Point vet[] = {Point::ZERO, Point(width, 0), Point(width, width), Point(0, width)};
    drawNode->drawPolygon(vet, 4, Color4F(), 2, Color4F::GRAY);
    latClear->addChild(drawNode);
    latClear->setPosition(Point(width * 2, 0));
    addChild(latClear);
}

void ManualDisplay::displayUIText()
{
    float posY = getContentSize().height - 80;
    Point pos = Point(150, posY);
    
    // text with system font
    auto txt = Text::create("Text System", "Marker Felt", 30);
    txt->setColor(Color3B::MAGENTA);
    txt->setPosition(pos);
    txt->setTouchScaleChangeEnabled(true);
    txt->setBright(true);
    addChild(txt);
    
    pos.y -= 100;
    
    // text with BMFont (520)
    auto txtBMF = TextBMFont::create("520", "font_vip.fnt");
    txtBMF->setPosition(pos);
    addChild(txtBMF);
    
    pos.y -= 100;
    
    // text with atlas (1314)
    auto txtAtlas = TextAtlas::create("1314", "font_power.png", 27, 28, "0");
    txtAtlas->setPosition(pos);
    addChild(txtAtlas);
    
    pos = Point(400, posY);
    Size sizeTF = Size(300, 40);
    
    // text field
    auto tfAcc = TextField::create("TF Input Account", "Helvetica", 30);
    tfAcc->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT); // effective
    tfAcc->setColor(Color3B::BLUE);
    tfAcc->setSize(sizeTF);
    tfAcc->setTouchSize(sizeTF);
    tfAcc->setTouchAreaEnabled(true); // necessary
    tfAcc->setPosition(pos);
    tfAcc->setMaxLength(12);
    tfAcc->setMaxLengthEnabled(true); // necessary
    pos.y -= 50;
    auto tfPsw = TextField::create("TF Input Password", "Helvetica", 30);
    tfPsw->setTextHorizontalAlignment(TextHAlignment::LEFT); // invalid
    tfPsw->setColor(Color3B::BLUE);
    tfPsw->setPasswordEnabled(true);
    tfPsw->setPasswordStyleText("o");
    tfPsw->setPosition(pos);
    addChild(tfAcc);
    addChild(tfPsw);
    
    pos.y -= 100;
    
    // edit box (with background)
    auto ebAcc = EditBox::create(sizeTF, Scale9Sprite::create("white.png", Rect(0, 0, sizeTF.width, sizeTF.height)));
    ebAcc->setColor(Color3B::GRAY);
    ebAcc->setPlaceHolder("EB Input Account");
    ebAcc->setFont("Marker Felt", 30);
    ebAcc->setFontColor(Color3B::BLUE);
    ebAcc->setPosition(pos);
    pos.y -= 50;
    auto ebPsw = EditBox::create(sizeTF, Scale9Sprite::create("white.png", Rect(0, 0, sizeTF.width, sizeTF.height)));
    ebPsw->setColor(Color3B::GRAY);
    ebPsw->setPlaceHolder("EB Input Password");
    ebPsw->setFont("Helvetica", 30);
    ebPsw->setFontColor(Color3B::BLUE);
    ebPsw->setInputMode(EditBox::InputMode::EMAIL_ADDRESS);
    ebPsw->setInputFlag(EditBox::InputFlag::PASSWORD);
    ebPsw->setPosition(pos);
    ebPsw->setMaxLength(12);
    addChild(ebAcc);
    addChild(ebPsw);
    
    pos.y -= 100;
    
    // rich text
    auto rtxt = RichText::create();
    rtxt->ignoreContentAdaptWithSize(false);
    rtxt->setSize(Size(700, 200));
    
    auto reTxt1 = RichElementText::create(1, Color3B::RED, 255, "[System] ", "Marker Felt", 30);
    auto reTxt2 = RichElementText::create(2, Color3B::BLACK, 255, "Congratulations! ", "Helvetica", 30);
    auto reImg3 = RichElementImage::create(3, Color3B::WHITE, 160, "star.png");
    
    auto btnName = ButtonText::create("Ghost ", Color3B::GREEN, "Helvetica", 30);
    BIND_LISTENER_TOUCH(btnName, this, ManualDisplay::onTouchRichText);
    auto reNode4 = RichElementCustomNode::create(4, Color3B::WHITE, 255, btnName);
    
    auto reTxt5 = RichElementText::create(5, Color3B::BLACK, 255, "have got a sword ", "Helvetica", 30);
    
    auto btnWeapon = ButtonText::create("ENDLESS ", Color3B::ORANGE, "Helvetica", 30);
    BIND_LISTENER_TOUCH(btnWeapon, this, ManualDisplay::onTouchRichText);
    auto reNode6 = RichElementCustomNode::create(4, Color3B::WHITE, 255, btnWeapon);
    
    auto reTxt7 = RichElementText::create(7, Color3B::BLACK, 255, "from the battle ", "Helvetica", 30);
    
    auto btnField = ButtonText::create("warcraft ", Color3B::MAGENTA, "Helvetica", 30);
    BIND_LISTENER_TOUCH(btnField, this, ManualDisplay::onTouchRichText);
    auto reNode8 = RichElementCustomNode::create(4, Color3B::WHITE, 255, btnField);
    
    auto reImg9 = RichElementImage::create(9, Color3B::GREEN, 160, "CloseNormal.png");
    
    rtxt->pushBackElement(reTxt1);
    rtxt->pushBackElement(reTxt2);
    rtxt->pushBackElement(reImg3);
    rtxt->pushBackElement(reNode4);
    rtxt->pushBackElement(reTxt5);
    rtxt->pushBackElement(reNode6);
    rtxt->pushBackElement(reTxt7);
    rtxt->pushBackElement(reNode8);
    rtxt->insertElement(reImg9, 3);
    
    rtxt->setSizeType(SIZE_PERCENT);
    rtxt->setSizePercent(Point(0.8, 0.3));
    rtxt->setPosition(Point(getContentSize().width / 2, 80));
    addChild(rtxt);
    
    // rich text show
    auto rxShow = Label::createWithTTF("RichText show", "pacifico.ttf", 30);
    rxShow->setColor(Color3B::GRAY);
    rxShow->setPosition(Point(getContentSize().width / 2, 40));
    rxShow->setTag(Tag::TAG_SELECTSHOW);
    addChild(rxShow);
    
    // !!!: show richtext touch event in onTouchRichText
}

void ManualDisplay::displayUIImageView()
{
    auto nodeSprites = Node::create();
    Point posCenter = NODE_CENTER(this);
    
    // imageView with local png (flower)
    auto imv = ImageView::create("flower.png");
    imv->setPosition(posCenter + Point(50, 150));
    nodeSprites->addChild(imv);
    
    LOADPLIST("10003_move.plist");
    
    // imageView with plist (runner)
    auto imvPlist = ImageView::create("10003_move_0000.png", UI_TEX_TYPE_PLIST);
    imvPlist->setPosition(posCenter + Point(200, -50));
    imvPlist->setTag(Tag::TAG_IMAGEVIEW);
    nodeSprites->addChild(imvPlist);
    
    // imageView with small rect
    auto imvRect = ImageViewExt::create("dragon.png", Rect(0.5, 0, 0.5, 1), SIZE_PERCENT);
    imvRect->setPosition(Point(250, 250));
    nodeSprites->addChild(imvRect);
    
    nodeSprites->setTag(Tag::TAG_NODESPRITE);
    addChild(nodeSprites);
    
    // !!!: update texture(runner) in update
    // !!!: loadTexture, not runAction
    
    // add sprite controller
    addSpriteController();
    
    // !!!: change ImageView attribute by controller
}

void ManualDisplay::displayUIButton()
{
    const int MenuAmount = 6;
    const string Menu[] = {"NewGame", "Continue", "Highscore", "Help", "About", "MoreGame"};
    LOADPLIST("menu.plist");
    Point pos = Point(250, getContentSize().height - 150);
    
    // button image (left)
    for (int i = 0; i < MenuAmount; i++) {
        string frmNormal = StringUtils::format("menu_%i_%s.png", i, "n");
        string frmSelect = StringUtils::format("menu_%i_%s.png", i, "s");
        string frmDisable = StringUtils::format("menu_%i_%s.png", i, "d");
        
        auto btnItem = Button::create();
        btnItem->loadTextureNormal(frmNormal, UI_TEX_TYPE_PLIST);
        btnItem->loadTexturePressed(frmSelect, UI_TEX_TYPE_PLIST);
        btnItem->loadTextureDisabled(frmDisable, UI_TEX_TYPE_PLIST);
        btnItem->setName(Menu[i].c_str());
        BIND_LISTENER_TOUCH(btnItem, this, ManualDisplay::onTouchButton);
        btnItem->setPosition(pos);
        btnItem->setTag(i);
        addChild(btnItem);
        
        pos.y -= 60;
        if (i == 1) {
            WIDGET_ENABEL(btnItem, false);
        }
    }
    
    pos = Point(getContentSize().width - 250, getContentSize().height - 150);
    
    // button text (right)
    for (int i = 0; i < MenuAmount; i++) {
        auto btnItem = ButtonText::create(Menu[i], Color3B::GREEN, "Marker Felt", 30);
        btnItem->setName(Menu[i].c_str());
        BIND_LISTENER_TOUCH(btnItem, this, ManualDisplay::onTouchButton);
        btnItem->setPosition(pos);
        addChild(btnItem);
        
        pos.y -= 60;
    }
    
    // label to show info
    auto labShow = Label::createWithTTF("Touch Button", "pacifico.ttf", 30);
    labShow->setColor(Color3B::RED);
    labShow->setPosition(Point(getContentSize().width / 2, getContentSize().height - 60));
    labShow->setTag(Tag::TAG_BUTTONTITLE);
    addChild(labShow);
}

void ManualDisplay::displayUILoadingBar()
{
    LOADPLIST("ui.plist");
    Point pos = NODE_CENTER(this) + Point(0, 180);
    float pad = 70;
    
    // loadingBar from left to right (row 1)
    auto ldbLeft = LoadingBar::create();
    ldbLeft->loadTexture("lodingFg.png", UI_TEX_TYPE_PLIST);
    ldbLeft->setDirection(LoadingBarTypeLeft);
    ldbLeft->setTag(Tag::TAG_LOADINGLEFT);
    ldbLeft->setPosition(pos);
    addChild(ldbLeft);
    
    pos.y -= pad;
    
    // loadingBar from right to left (row 2)
    auto ldbRight = LoadingBar::create();
    ldbRight->loadTexture("lodingFg.png", UI_TEX_TYPE_PLIST);
    ldbRight->setDirection(LoadingBarTypeRight);
    ldbRight->setTag(Tag::TAG_LOADINGRIGHT);
    ldbRight->setPosition(pos);
    addChild(ldbRight);
    
    pos.y -= pad;
    
    // loadingBar ext from left to right (row 3)
    auto ldbExtL = LoadingBarExt::create("lodingBg.png", "lodingFg.png", UI_TEX_TYPE_PLIST);
    ldbExtL->setDirection(LoadingBarTypeLeft);
    ldbExtL->setTag(Tag::TAG_LOADINGEXTL);
    ldbExtL->setPosition(pos);
    addChild(ldbExtL);
    
    pos.y -= pad;
    
    // loadingBar ext from right to left with label (row 4)
    auto ldbExtR = LoadingBarExt::create("lodingBg.png", "lodingFg.png", UI_TEX_TYPE_PLIST, 0, "Marker Felt", Color3B::GREEN, 30);
    ldbExtR->setDirection(LoadingBarTypeRight);
    ldbExtR->setTag(Tag::TAG_LOADINGEXTR);
    ldbExtR->setPosition(pos);
    addChild(ldbExtR);
    
    pos.y -= pad;
    
    // loadingBar ext from left to right with label under bar (row 5)
    auto ldbExtLL = LoadingBarExt::create("lodingBg.png", "lodingFg.png", UI_TEX_TYPE_PLIST);
    Label * lab = Label::createWithSystemFont("0/100", "pacifico.ttf", 30);
    lab->setColor(Color3B::RED);
    lab->enableShadow();
    ldbExtLL->setLabelProgress(lab);
    ldbExtLL->setLabelProgressPosition(Point(0.5, -0.5), SIZE_PERCENT);
    ldbExtLL->setLabelProgressVisible(true);
    ldbExtLL->setDirection(LoadingBarTypeLeft);
    ldbExtLL->setTag(Tag::TAG_LOADINGEXTLL);
    ldbExtLL->setPosition(pos);
    addChild(ldbExtLL);
    
    pos.y -= pad;
    
    // loadingBar ext from right to left with callback (row 6)
    auto ldbExt = LoadingBarExt::create("lodingBg.png", "lodingFg.png", UI_TEX_TYPE_PLIST);
    ldbExt->setDirection(LoadingBarTypeRight);
    ldbExt->setTag(Tag::TAG_LOADINGEXT);
    ldbExt->setPosition(pos);
    ldbExt->setCallBack([&](Ref * pSender) {
        LoadingBarExt * loadingBar = static_cast<LoadingBarExt *>(pSender);
        ParticleExplosion * eff = static_cast<ParticleExplosion *>(loadingBar->getChildByTag(Tag::TAG_EFFECT));
        if (!eff) {
            eff = ParticleExplosion::create();
            eff->setPosition(NODE_CENTER(loadingBar));
            eff->setTag(Tag::TAG_EFFECT);
            loadingBar->addChild(eff);
        }
        eff->resetSystem();
    });
    addChild(ldbExt);
}

void ManualDisplay::displayUICheckBox()
{
    LOADPLIST("ui.plist");
    Point pos = NODE_CENTER(this) + Point(0, 150);
    float pad = 70;
    
    // checkBox tick (tick)
    auto cbTick = CheckBox::create();
    cbTick->loadTextureBackGround("checkBg.png", UI_TEX_TYPE_PLIST);
    cbTick->loadTextureBackGroundSelected("checkBg.png", UI_TEX_TYPE_PLIST);
    cbTick->loadTextureFrontCross("checkFg.png", UI_TEX_TYPE_PLIST);
    cbTick->setSelectedState(true);
    cbTick->setPosition(pos);
    cbTick->setName("Tick CheckBox");
    BIND_LISTENER_CB(cbTick, this, ManualDisplay::onTouchCheckBox);
    addChild(cbTick);
    
    pos.y -= pad;
    
    // checkBox star (star)
    auto cbStar = CheckBox::create("checBg.png", "checBg.png", "checFg.png", "checFg.png", "checFg.png", UI_TEX_TYPE_PLIST);
    cbStar->setSelectedState(true);
    cbStar->setPosition(pos);
    cbStar->setName("Star CheckBox");
    BIND_LISTENER_CB(cbStar, this, ManualDisplay::onTouchCheckBox);
    addChild(cbStar);
    
    pos.y -= pad;
    
    // checkBox tick with title (Accept the conditions)
    auto cbTitle = CheckBoxExt::create("checkBg.png", "checkFg.png", UI_TEX_TYPE_PLIST,
                                       "Marker Felt", Color3B::BLUE, 30, "Accept the conditions");
    cbTitle->setPosition(pos);
    cbTitle->setName("Accept the conditions");
    cbTitle->setTag(Tag::TAG_CHECKBOXTICK);
    BIND_LISTENER_CB(cbTitle, this, ManualDisplay::onTouchCheckBox);
    addChild(cbTitle);
    
    pos.y -= pad;
    
    // combination (combination)
    auto labTitle = Label::createWithTTF("Combination", "pacifico.ttf", 30);
    labTitle->setColor(Color3B::GREEN);
    auto cbComb = CheckBoxExt::create(cbStar, labTitle);
    cbComb->setPosition(pos);
    cbComb->setName("Combination");
    cbComb->setTag(Tag::TAG_CHECKBOXSTAR);
    BIND_LISTENER_CB(cbComb, this, ManualDisplay::onTouchCheckBox);
    addChild(cbComb);
    
    // radio group (left)
    const string Menu[] = {"NewGame", "Continue", "Highscore", "Help", "About", "MoreGame"};
    pos = Point(100, getContentSize().height - 150);
    auto radioGroup = RadioGroup::create("checkBg.png", "checkFg.png", UI_TEX_TYPE_PLIST, "Marker Felt", Color3B::BLACK, 30, 6, Menu,
                                         [&](Ref * pSender) {
                                             int index = dynamic_cast<RadioGroup *>(pSender)->getSelectIndex();
                                             CheckBoxExt * cbExt = dynamic_cast<RadioGroup *>(pSender)->getSelectItem();
                                             string tip = StringUtils::format("Radio at %i [ %s ]", index, cbExt->getName());
                                             auto label = getChildByTag(Tag::TAG_SELECTSHOW);
                                             if (label) {
                                                 dynamic_cast<Label *>(label)->setString(tip);
                                             }
                                         });
    radioGroup->setPosition(pos);
    addChild(radioGroup);
    
    // label to show info
    auto label = Label::createWithSystemFont("Check Show", "Marker Felt", 30);
    label->setTag(Tag::TAG_SELECTSHOW);
    label->setColor(Color3B::MAGENTA);
    label->setPosition(Point(getContentSize().width / 2, 50));
    addChild(label);
}

void ManualDisplay::displayUISlide()
{
    LOADPLIST("ui.plist");
    Point pos = NODE_CENTER(this) + Point(-200, 180);
    float pad = 70;
    
    // slider normal (left row 1)
    auto sld = Slider::create();
    sld->loadBarTexture("sliderBg.png", UI_TEX_TYPE_PLIST);
    sld->loadSlidBallTextures("sliderFg.png", "sliderFg.png", "sliderFg.png", UI_TEX_TYPE_PLIST);
    sld->setPercent(50);
    sld->setPosition(pos);
    addChild(sld);
    
    pos.y -= pad;
    
    // slider with progress bar (left row 2)
    auto sldPrg = Slider::create();
    sldPrg->loadBarTexture("sliderBg.png", UI_TEX_TYPE_PLIST);
    sldPrg->loadProgressBarTexture("switchFg.png", UI_TEX_TYPE_PLIST);
    sldPrg->loadSlidBallTextures("sliderFg.png", "sliderFg.png", "sliderFg.png", UI_TEX_TYPE_PLIST);
    sldPrg->setPercent(50);
    sldPrg->setPosition(pos);
    sldPrg->setName("SliderBar");
    BIND_LISTENER_SL(sldPrg, this, ManualDisplay::onTouchSlider);
    addChild(sldPrg);
    
    pos = Point(120, 200);
    
    // vertical slider with progress bar (left row 3 col 1)
    // also control another 2 slider
    auto sldPrgAll = Slider::create();
    sldPrgAll->loadBarTexture("sliderBg.png", UI_TEX_TYPE_PLIST);
    sldPrgAll->loadProgressBarTexture("switchFg.png", UI_TEX_TYPE_PLIST);
    sldPrgAll->loadSlidBallTextures("sliderFg.png", "sliderFg.png", "sliderFg.png", UI_TEX_TYPE_PLIST);
    sldPrgAll->setPercent(50);
    sldPrgAll->setPosition(pos);
    sldPrgAll->setRotation(-90);
    sldPrgAll->setTag(Tag::TAG_SLIDERALL);
    sldPrgAll->setName("sldPrgAll");
    BIND_LISTENER_SL(sldPrgAll, this, ManualDisplay::onTouchSlider);
    addChild(sldPrgAll);
    
    pos.x += pad;
    
    // vertical slider with progress bar (left row 3 col 2)
    auto sldPrgLeft = Slider::create();
    sldPrgLeft->loadBarTexture("sliderBg.png", UI_TEX_TYPE_PLIST);
    sldPrgLeft->loadProgressBarTexture("switchFg.png", UI_TEX_TYPE_PLIST);
    sldPrgLeft->loadSlidBallTextures("sliderFg.png", "sliderFg.png", "sliderFg.png", UI_TEX_TYPE_PLIST);
    sldPrgLeft->setPercent(50);
    sldPrgLeft->setPosition(pos);
    sldPrgLeft->setRotation(-90);
    sldPrgLeft->setTag(Tag::TAG_SLIDERLEFT);
    sldPrgLeft->setName("sldPrgLeft");
    BIND_LISTENER_SL(sldPrgLeft, this, ManualDisplay::onTouchSlider);
    addChild(sldPrgLeft);
    
    pos.x += pad;
    
    // vertical slider with progress bar (left row 3 col 3)
    auto sldPrgRight = Slider::create();
    sldPrgRight->loadBarTexture("sliderBg.png", UI_TEX_TYPE_PLIST);
    sldPrgRight->loadProgressBarTexture("switchFg.png", UI_TEX_TYPE_PLIST);
    sldPrgRight->loadSlidBallTextures("sliderFg.png", "sliderFg.png", "sliderFg.png", UI_TEX_TYPE_PLIST);
    sldPrgRight->setPercent(50);
    sldPrgRight->setPosition(pos);
    sldPrgRight->setRotation(-90);
    sldPrgRight->setTag(Tag::TAG_SLIDERRIGHT);
    sldPrgRight->setName("sldPrgRight");
    BIND_LISTENER_SL(sldPrgRight, this, ManualDisplay::onTouchSlider);
    addChild(sldPrgRight);
    
    pos = NODE_CENTER(this) + Point(150, 180);
    
    // slider bar with fine progress (right row 1)
    auto sldFine = SliderFine::create("sliderBg.png", "sliderFg.png", "", UI_TEX_TYPE_PLIST);
    sldFine->setPercent(50);
    sldFine->setPosition(pos);
    addChild(sldFine);
    
    pos.y -= pad;
    
    // set fine position and fine is touch disable (right row 2)
    auto sldFine2 = SliderFine::create("sliderBg.png", "sliderFg.png", "switchFg.png", UI_TEX_TYPE_PLIST, "Marker Felt", Color3B::GREEN, 30);
    sldFine2->setPercent(50);
    sldFine2->setTextFieldTouchEnable(false);
    sldFine2->setTextFieldPosition(Point(0.5, -0.8), SIZE_PERCENT);
    sldFine2->setPosition(pos);
    addChild(sldFine2);
    
    pos.y -= pad;
    
    // slider bar with check box (right row 3)
    auto sldCheck = SliderCheck::create("sliderBg.png", "sliderFg.png", "", "checkBg.png", "checkFg.png", UI_TEX_TYPE_PLIST);
    sldCheck->setPercent(50);
    sldCheck->setPosition(pos);
    addChild(sldCheck);
    
    pos.y -= pad;
    
    // set check postion (right row 4)
    auto sldCheck2 = SliderCheck::create("sliderBg.png", "sliderFg.png", "switchFg.png", "checkBg.png", "checkFg.png", UI_TEX_TYPE_PLIST);
    sldCheck2->setPercent(50);
    sldCheck2->setCheckBoxPosition(Point(1.2, 0.5), SIZE_PERCENT);
    sldCheck2->setPosition(pos);
    addChild(sldCheck2);
    
    pos.y -= pad;
    
    // combination (right row 5)
    auto textField = TextField::create("combination", "Marker Felt", 30);
    textField->setColor(Color3B::ORANGE);
    textField->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    auto sldComb = SliderFine::create(sld, textField);
    sldComb->setPosition(pos);
    addChild(sldComb);
    
    // label to show info
    auto label = Label::createWithSystemFont("Slider Show", "Marker Felt", 30);
    label->setTag(Tag::TAG_SELECTSHOW);
    label->setColor(Color3B::MAGENTA);
    label->setPosition(Point(getContentSize().width / 2, 50));
    addChild(label);
}

void ManualDisplay::displayUIScrollView()
{
    Size size = Size(300, 400);
    Point pos = Point(100, 200);
    
    // scrollView with out bounce (top left)
    auto sv = ui::ScrollView::create();
    sv->setSize(size);
    sv->setPosition(pos);
    auto sprMan = Sprite::create("man.png");
    auto imvWolf = ImageView::create("wolf.png");
    sprMan->setPosition(Point(0, 0));
    imvWolf->setPosition(Point(0, sprMan->getContentSize().height));
    sv->addChild(sprMan);
    sv->addChild(imvWolf);
    float innerHeight = sprMan->getContentSize().height + imvWolf->getContentSize().height;
    sv->setInnerContainerSize(Size(size.width, innerHeight)); // necessary
    addChild(sv);
    
    pos.x += 500;
    
    // scrollView with bounce (top right)
    auto svBounce = ui::ScrollView::create();
    svBounce->setSize(size);
    svBounce->setPosition(pos);
    svBounce->setDirection(SCROLLVIEW_DIR_VERTICAL); // default
    svBounce->setBounceEnabled(true); // bounce
    auto sprFlower = Sprite::create("flower.png");
    auto imvStar = ImageView::create("star.png");
    sprFlower->setPosition(Point(0, 0));
    imvStar->setPosition(Point(0, sprFlower->getContentSize().height));
    svBounce->addChild(sprFlower);
    svBounce->addChild(imvStar);
    innerHeight = sprFlower->getContentSize().height + imvStar->getContentSize().height;
    svBounce->setInnerContainerSize(Size(size.width, innerHeight)); // necessary
    addChild(svBounce);
    
    auto svBoth = ui::ScrollView::create();
    svBoth->setSize(Size(300, 300));
    svBoth->setAnchorPoint(Point::ANCHOR_MIDDLE);
    svBoth->setPosition(NODE_CENTER(this) + Point(0, 100));
    svBoth->setDirection(SCROLLVIEW_DIR_BOTH);
    auto sprDragon = Sprite::create("dragon.png");
    sprDragon->setScale(2);
    sprDragon->setPosition(NODE_CENTER(svBoth));
    svBoth->addChild(sprDragon);
    svBoth->setInnerContainerSize(sprDragon->getContentSize());
    svBoth->jumpToPercentBothDirection(Point(50, 50));
    BIND_LISTENER_SV(svBoth, this, ManualDisplay::onTouchScollView); // add event listener
    addChild(svBoth);
    
    // scrollView HORIZONTAL (bottom)
    auto svH = ui::ScrollView::create();
    svH->setSize(Size(600, 100));
    svH->setPosition(Point(100, 50));
    pos = Point(32, 32);
    for (int i = 0; i < 10; i++) {
        auto btn = Button::create("white.png");
        btn->setColor(Color3B::GRAY);
        btn->setTitleText(StringUtils::format("%i", i + 1));
        btn->setTitleColor(Color3B::GREEN);
        btn->setTitleFontSize(25);
        btn->setPosition(pos);
        svH->addChild(btn);
        pos.x += 80;
    }
    svH->setInnerContainerSize(Size(800, 100));
    svH->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
    svH->setBounceEnabled(true);
    addChild(svH);
    
    // label to show info
    auto label = Label::createWithSystemFont("Scroll Show", "Marker Felt", 30);
    label->setTag(Tag::TAG_SELECTSHOW);
    label->setColor(Color3B::MAGENTA);
    label->setPosition(Point(getContentSize().width / 2, svBoth->getPosition().y - 190));
    addChild(label);
}

void ManualDisplay::displayUIListView()
{
    const int ListAmount = 20;
    Point pos = NODE_CENTER(this);
    Size size = Size(200, 300);

    // list normal (left)
    auto list = ListView::create();
    for (int i = 0; i < ListAmount; i++) {
        string title = StringUtils::format("ListItem-%i", (i + 1));
        auto label = Text::create(title, "Marker Felt", 30);
        label->setColor(Color3B::BLUE);
        label->ignoreContentAdaptWithSize(false);
        label->setSize(Size(200, 30));
        list->pushBackCustomItem(label);
    }
    list->setAnchorPoint(Point::ANCHOR_MIDDLE);
    list->ignoreAnchorPointForPosition(false);
    list->setSize(size);
    list->setPosition(pos - Point(250, 0));
    list->setTouchEnabled(true);
    BIND_LISTENER_LV(list, this, ManualDisplay::onTouchListView);
    addChild(list);
    
    // list with color bg (middle)
    auto listColor = ListView::create();
    for (int i = 0; i < ListAmount; i++) {
        string title = StringUtils::format("ListItem-%i", (i + 1));
        auto label = Text::create(title, "Marker Felt", 30);
        label->setColor(Color3B::GREEN);
        listColor->pushBackCustomItem(label);
    }
    listColor->setAnchorPoint(Point::ANCHOR_MIDDLE);
    listColor->setSize(size);
    listColor->setBounceEnabled(true);
    listColor->setBackGroundColorType(LAYOUT_COLOR_GRADIENT);
    listColor->setBackGroundColor(Color3B::YELLOW, Color3B::RED);
    listColor->setPosition(pos);
    BIND_LISTENER_LV(listColor, this, ManualDisplay::onTouchListView);
    addChild(listColor);
    
    // list with image bg (right)
    auto listImg = ListView::create();
    for (int i = 0; i < ListAmount; i++) {
        string title = StringUtils::format("ListItem-%i", (i + 1));
        auto label = Text::create(title, "Marker Felt", 30);
        label->setColor(Color3B::MAGENTA);
        listImg->pushBackCustomItem(label);
    }
    listImg->setAnchorPoint(Point::ANCHOR_MIDDLE);
    listImg->setSize(size);
    listImg->setBounceEnabled(true);
    listImg->setBackGroundImage("flower.png");
    listImg->setPosition(pos + Point(250, 0));
    listImg->setClippingType(LAYOUT_CLIPPING_STENCIL); // default
    BIND_LISTENER_LV(listImg, this, ManualDisplay::onTouchListView); // add event listener
    addChild(listImg);

    // label to show info
    auto label = Label::createWithSystemFont("#BUG-ListenerDisable", "Marker Felt", 30);
    label->setTag(Tag::TAG_SELECTSHOW);
    label->setColor(Color3B::RED);
    label->setPosition(Point(getContentSize().width / 2, 50));
    addChild(label);
}

void ManualDisplay::displayUIPageView()
{
    Size size = Size(300, 300);
    Point pos = NODE_CENTER(this) - Point(200, 0);
    
    // pageview with color layout
    auto pvLayout = PageView::create();
    pvLayout->ignoreAnchorPointForPosition(false); // default is true
    pvLayout->setAnchorPoint(Point::ANCHOR_MIDDLE);
    pvLayout->setSize(size);
    for (int i = 0; i < 8; i++) {
        Color3B color = Color3B(RANDOM_CC(0, 255), RANDOM_CC(0, 255), RANDOM_CC(0, 255));
        auto layout = Layout::create();
        layout->setSize(size);
        layout->setBackGroundColor(color);
        layout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
        pvLayout->addPage(layout);
    }
    pvLayout->setPosition(pos);
    addChild(pvLayout);
    
    pos.x += 400;
    LOADPLIST("10003_move.plist");
    
    // pageview add widget
    auto pvWidget = PageView::create();
    pvWidget->ignoreAnchorPointForPosition(false); // default is true
    pvWidget->setAnchorPoint(Point::ANCHOR_MIDDLE);
    pvWidget->setSize(size);
    for (int i = 0; i < 8; i++) {
        string file = StringUtils::format("10003_move_000%i.png", i);
        auto imv = ImageView::create(file.c_str(), UI_TEX_TYPE_PLIST);
        imv->setPosition(NODE_CENTER(pvWidget));
        pvWidget->addWidgetToPage(imv, i, true);
    }
    pvWidget->setPosition(pos);
    BIND_LISTENER_PV(pvWidget, this, ManualDisplay::onTouchPageView); // add event listener
    addChild(pvWidget);
    
    // label to show info
    auto label = Label::createWithSystemFont("Pageview Show", "Marker Felt", 30);
    label->setTag(Tag::TAG_SELECTSHOW);
    label->setColor(Color3B::MAGENTA);
    label->setPosition(Point(getContentSize().width / 2, 50));
    addChild(label);
}

void ManualDisplay::displayUIBox()
{
    // HBox (bottom)
    auto hbox = HBox::create();
    for (int i = 0; i < 5; i++) {
        Color3B color = Color3B(RANDOM_CC(0, 255), RANDOM_CC(0, 255), RANDOM_CC(0, 255));
        auto btn = Button::create("white.png");
        btn->setColor(color);
        btn->ignoreContentAdaptWithSize(false);
        btn->setSize(Size(RANDOM_CC(50, 100), 64));
        hbox->addChild(btn);
    }
    hbox->setPosition(Point(200, 150));
    addChild(hbox);
    
    // VBox (left)
    auto vbox = VBox::create();
    for (int i = 0; i < 5; i++) {
        Color3B color = Color3B(RANDOM_CC(0, 255), RANDOM_CC(0, 255), RANDOM_CC(0, 255));
        auto btn = Button::create("white.png");
        btn->setColor(color);
        btn->ignoreContentAdaptWithSize(false);
        btn->setSize(Size(64, RANDOM_CC(50, 100)));
        vbox->addChild(btn);
    }
    vbox->setPosition(Point(100, 450));
    addChild(vbox);
    
    // Relative Box (center)
    auto rbox = RelativeBox::create();
    for (int i = 0; i < 5; i++) {
        Color3B color = Color3B(RANDOM_CC(0, 255), RANDOM_CC(0, 255), RANDOM_CC(0, 255));
        auto btn = Button::create("white.png");
        btn->setColor(color);
        btn->ignoreContentAdaptWithSize(false);
        btn->setSize(Size((5 - i) * 50, (5 - i) * 50));
        rbox->addChild(btn);
    }
    rbox->setPosition(Point(200, 450));
    addChild(rbox);
}

void ManualDisplay::displayCCBuilder()
{
    auto layer = CCBLayer::layer();
    layer->ignoreAnchorPointForPosition(false);
    layer->setPosition(NODE_CENTER(this));
    addChild(layer);
}

void ManualDisplay::displayCCStudio()
{
    auto layer = CCSLayer::layer();
    layer->setPosition(Point(-120, -100));
    addChild(layer);
    
    /* Replace res.plist and res.png export from CocosStudio 
     with res.plist and res.pvr.ccz publish from TexturePacker */
}

void ManualDisplay::displayTileMap()
{
    auto tileMap = TMXTiledMap::create("test.tmx");
    addChild(tileMap);
    
    auto objGroup = tileMap->getObjectGroup("tables");
    if (objGroup) {
        auto objRect = objGroup->getObject("objRect");
        float x = objRect["x"].asFloat();
        float y = objRect["y"].asFloat();
        float w = objRect["width"].asFloat();
        float h = objRect["height"].asFloat();
        int r = objRect["r"].asInt();
        int g = objRect["g"].asInt();
        int b = objRect["b"].asInt();
        int a = objRect["a"].asInt();
        
        auto drawNode = DrawNodeExt::create();
        drawNode->drawRect(Rect(x, y, w, h), 2, Color4F(Color4B(r, g, b, a)));
        tileMap->addChild(drawNode);
    }
}

void ManualDisplay::displayParticle()
{
    // black bg
    auto bg = Layout::create();
    bg->ignoreContentAdaptWithSize(false);
    bg->setSize(getSize());
    bg->setBackGroundColor(Color3B::BLACK);
    bg->setBackGroundColorType(LAYOUT_COLOR_SOLID);
    addChild(bg);
    
    // rain
    auto pRain = ParticleRain::create();
    addChild(pRain);
    
    // snow
    auto pSnow = ParticleSnow::create();
    addChild(pSnow);
    
    // fire
    auto pFire = ParticleFire::create();
    addChild(pFire);
    
    // sun
    auto pSun = ParticleSun::create();
    addChild(pSun);
    
    // galaxy
    auto pGalaxy = ParticleGalaxy::create();
    pGalaxy->setPosition(Point(200, 200));
    addChild(pGalaxy);
    
    // flower
    auto pFlower = ParticleFlower::create();
    pFlower->setPosition(Point(200, 400));
    addChild(pFlower);
    
    // meteor
    auto pMeteor = ParticleMeteor::create();
    Point posStart = Point(0, 800);
    Point posEnd = Point(900, -400);
    pMeteor->setPosition(posStart);
    pMeteor->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveTo::create(3, posEnd), Place::create(posStart))));
    addChild(pMeteor);
    
    // spirial
    auto pSpiral = ParticleSpiral::create();
    pSpiral->setPosition(Point(300, 300));
    addChild(pSpiral);
    
    // explosion
    auto pExplosion = ParticleExplosion::create();
    pExplosion->setPosition(Point(300, 300));
    addChild(pExplosion);
    
    // smoke
    auto pSmoke = ParticleSmoke::create();
    pSmoke->setPosition(Point(600, 100));
    addChild(pSmoke);
    
    // particle from plist
    auto pDIY = CCParticleSystemQuad::create("eliminate.plist");
    if (pDIY) {
        pDIY->setPosition(Point(600, 300));
        pDIY->setStartColor(Color4F(Color3B::GREEN));
        pDIY->setEndColor(Color4F(Color3B::ORANGE));
        addChild(pDIY);
    }
    
    // self particle with action (useful)
    auto btnRect = Button::create("white.png");
    btnRect->ignoreContentAdaptWithSize(false);
    btnRect->setSize(Size(100, 100));
    btnRect->setPosition(Point(650, 350));
    btnRect->setColor(Color3B::GREEN);
    btnRect->setTitleText("👻");
    btnRect->setTitleFontSize(50);
    addChild(btnRect);
    
    auto pFocus = CCParticleSystemQuad::create("focus.plist");
    if (pFocus) {
        auto pArray = PointArray::create(4);
        pArray->addControlPoint(Point(600 + 5, 300 + 5));
        pArray->addControlPoint(Point(600 + 5, 400 - 5));
        pArray->addControlPoint(Point(700 - 5, 400 - 5));
        pArray->addControlPoint(Point(700 - 5, 300 + 5));
        pArray->addControlPoint(Point(600 + 5, 300 + 5));
        
        auto action = CardinalSplineTo::create(5, pArray, 0);
        pFocus->setPosition(Point(600 + 5, 300 + 5));
        pFocus->runAction(RepeatForever::create(action));
        addChild(pFocus);
    }
}

void ManualDisplay::displayAction()
{
    Point pos = NODE_CENTER(this);
    
    // add action player
    auto sprMan = Sprite::create("man.png");
    sprMan->setPosition(pos - Point(200, -100));
    addChild(sprMan, 0, Tag::TAG_ACTOR_MAN);
    
    auto imvWolf = ImageView::create("wolf.png");
    imvWolf->setPosition(pos + Point(200, 100));
    addChild(imvWolf, 0, Tag::TAG_ACTOR_WOLF);
    
    // add action controller
    addActionController();
    
    // !!!: change action by controller
}

void ManualDisplay::displayConnect()
{
    // Http request (top)
    string checkURL = "http://mszg.zoman.cn/GetVersion.jsp?version=58&appVersion=1.0&platform=2&device=2";
    float posX = getContentSize().width / 2;
    float posY = getContentSize().height - 50;
    
    auto labHttp = Label::createWithSystemFont("HttpRequest", "Marker Felt", 30);
    labHttp->setColor(Color3B::ORANGE);
    labHttp->setPosition(Point(posX, posY));
    addChild(labHttp);
    
    posY -= 60;
    
    auto labURL = Label::createWithSystemFont(checkURL, "Helvetica", 20);
    labURL->setColor(Color3B::BLUE);
    labURL->setPosition(Point(posX, posY));
    labURL->setDimensions(500, 60);
    addChild(labURL);
    
    posY -= 40;
    
    auto menuItem = MenuItemFont::create("Send");
    menuItem->setFontSize(40);
    menuItem->setColor(Color3B::BLUE);
    menuItem->setCallback([&](Ref * pSender) {
        auto request = new HttpRequest();
        if (request) {
            string checkURL = "http://mszg.zoman.cn/GetVersion.jsp?version=58&appVersion=1.0&platform=2&device=2";
            request->setUrl(checkURL.c_str());
            request->setRequestType(HttpRequest::Type::GET);
            request->setResponseCallback(this, httpresponse_selector(ManualDisplay::onHttpRequstCallback));
            request->setTag("Connect");
            HttpClient::getInstance()->send(request);
            request->release();
        }
    });
    auto menu = Menu::create(menuItem, nullptr);
    menu->setPosition(Point(posX, posY));
    addChild(menu);
    
    posY -= 50;
    
    auto labRet = Label::createWithSystemFont("response", "Helvetica", 20);
    labRet->setColor(Color3B::GRAY);
    labRet->setPosition(Point(posX, posY));
    labRet->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    labRet->setDimensions(500, 60);
    labRet->setTag(Tag::TAG_SELECTSHOW);
    addChild(labRet);
    
    posY -= 80;
    
    // TCP Socket (Bottom)
    auto labSocket = Label::createWithSystemFont("Socket", "Marker Felt", 30);
    labSocket->setColor(Color3B::ORANGE);
    labSocket->setPosition(Point(posX, posY));
    addChild(labSocket);
    
    posY -= 50;
    
    auto labIP = Label::createWithSystemFont("113.108.221.174:30001", "Helvetica", 30);
    labIP->setColor(Color3B::BLUE);
    labIP->setPosition(Point(posX, posY));
    addChild(labIP);
    
    posY -= 70;
    
    auto menuItemConn = MenuItemFont::create("Connect");
    menuItemConn->setFontSize(40);
    menuItemConn->setColor(Color3B::GREEN);
    menuItemConn->setCallback([&](Ref * pSender) {
        string ip = "113.108.221.174";
        int port = 30001;
        AppClient * client = AppConnect::client();
        if (tick > 0) {
            client->resumeConnect();
        } else {
            client->startServer(ip, port);
        }
        client->heartbeatEnabel = true;
        tick = 1;
        currFrame = 1;
    });
    auto menuItemStop = MenuItemFont::create("Stop");
    menuItemStop->setFontSize(40);
    menuItemStop->setColor(Color3B::RED);
    menuItemStop->setCallback([&](Ref * pSender) {
        currFrame = 0;
        AppConnect::stopConnect();
    });
    auto menuConn = Menu::create(menuItemConn, menuItemStop, nullptr);
    menuConn->setPosition(Point(posX, posY));
    menuConn->alignItemsVerticallyWithPadding(5);
    addChild(menuConn);
}

#pragma mark -

void ManualDisplay::onSwitchLayer(Ref *pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto layerMultiplex = getChildByTag(Tag::TAG_LAYERMULTIPLEX);
            CC_BREAK_IF(!layerMultiplex);
            dynamic_cast<LayerMultiplex *>(layerMultiplex)->switchTo(dynamic_cast<Button *>(pSender)->getTag());
            break;
        }
        default:
            break;
    }
    
}

void ManualDisplay::addSpriteController()
{
    Point pos = Point(40, 40);
    const float pad = 75;
    float fontSize = 14;
    
    // setColor
    const int ColorAmount = 4;
    const Color3B Color[ColorAmount] = {Color3B::WHITE, Color3B::RED, Color3B::GREEN, Color3B::BLUE};
    const string ColorName[ColorAmount] = {"Color", "RED", "GREEN", "BLUE"};
    for (int i = 0; i < ColorAmount; i++) {
        auto btnColor = Button::create("white.png");
        btnColor->setColor(Color[i]);
        btnColor->setPosition(pos);
        btnColor->setTitleText(ColorName[i].c_str());
        btnColor->setTitleFontSize(fontSize);
        btnColor->setTitleColor(Color3B::BLACK);
        BIND_LISTENER_TOUCH(btnColor, this, ManualDisplay::onSetSpriteColor);
        addChild(btnColor);
        
        pos.y += pad;
    }
    
    pos = Point(40, 40);
    pos.x += pad;

    // flip x & y
    auto btnFlipX = Button::create("white.png");
    btnFlipX->setColor(Color3B::GRAY);
    btnFlipX->setPosition(pos);
    btnFlipX->setTitleFontSize(fontSize);
    btnFlipX->setTitleColor(Color3B::BLACK);
    btnFlipX->setTitleText("FlipX");
    btnFlipX->setTag(0);
    BIND_LISTENER_TOUCH(btnFlipX, this, ManualDisplay::onSetSpriteFlip);
    addChild(btnFlipX);
    
    pos.x += pad;
    
    auto btnFlipY = Button::create("white.png");
    btnFlipY->setColor(Color3B::GRAY);
    btnFlipY->setPosition(pos);
    btnFlipY->setTitleFontSize(fontSize);
    btnFlipY->setTitleColor(Color3B::BLACK);
    btnFlipY->setTitleText("FlipY");
    btnFlipY->setTag(1);
    BIND_LISTENER_TOUCH(btnFlipY, this, ManualDisplay::onSetSpriteFlip);
    addChild(btnFlipY);
    
    pos.x += pad;

    // rotation
    auto btnRotate = Button::create("white.png");
    btnRotate->setColor(Color3B::GRAY);
    btnRotate->setPosition(pos);
    btnRotate->setTitleFontSize(fontSize);
    btnRotate->setTitleColor(Color3B::BLACK);
    btnRotate->setTitleText("Rotate");
    BIND_LISTENER_TOUCH(btnRotate, this, ManualDisplay::onSetSpriteRotate);
    addChild(btnRotate);
    
    pos.x += pad;

    // visible
    auto btnVisible = Button::create("white.png");
    btnVisible->setColor(Color3B::GRAY);
    btnVisible->setPosition(pos);
    btnVisible->setTitleFontSize(fontSize);
    btnVisible->setTitleColor(Color3B::BLACK);
    btnVisible->setTitleText("Visible");
    BIND_LISTENER_TOUCH(btnVisible, this, ManualDisplay::onSetSpriteVisible);
    addChild(btnVisible);
    
    pos.x += pad;
    
    // skew x & y
    auto btnSkewX = Button::create("white.png");
    btnSkewX->setColor(Color3B::GRAY);
    btnSkewX->setPosition(pos);
    btnSkewX->setTitleFontSize(fontSize);
    btnSkewX->setTitleColor(Color3B::BLACK);
    btnSkewX->setTitleText("SkewX");
    btnSkewX->setTag(0);
    BIND_LISTENER_TOUCH(btnSkewX, this, ManualDisplay::onSetSpriteSkew);
    addChild(btnSkewX);
    
    pos.x += pad;
    
    auto btnSkewY = Button::create("white.png");
    btnSkewY->setColor(Color3B::GRAY);
    btnSkewY->setPosition(pos);
    btnSkewY->setTitleFontSize(fontSize);
    btnSkewY->setTitleColor(Color3B::BLACK);
    btnSkewY->setTitleText("SkewY");
    btnSkewY->setTag(1);
    BIND_LISTENER_TOUCH(btnSkewY, this, ManualDisplay::onSetSpriteSkew);
    addChild(btnSkewY);
    
    pos.x += pad;
    
    // scale
    auto btnScale = Button::create("white.png");
    btnScale->setColor(Color3B::GRAY);
    btnScale->setPosition(pos);
    btnScale->setTitleFontSize(fontSize);
    btnScale->setTitleColor(Color3B::BLACK);
    btnScale->setTitleText("Scale");
    BIND_LISTENER_TOUCH(btnScale, this, ManualDisplay::onSetSpriteScale);
    addChild(btnScale);
    
    pos.x += pad;
    
    // opacity
    auto btnOpacity = Button::create("white.png");
    btnOpacity->setColor(Color3B::GRAY);
    btnOpacity->setPosition(pos);
    btnOpacity->setTitleFontSize(fontSize);
    btnOpacity->setTitleColor(Color3B::BLACK);
    btnOpacity->setTitleText("Opacity");
    BIND_LISTENER_TOUCH(btnOpacity, this, ManualDisplay::onSetSpriteOpacity);
    addChild(btnOpacity);
    
    pos.x += pad;
    
    // blend func
    if (manualType == ManualType::SPRITE) {
        auto btnBlend = Button::create("white.png");
        btnBlend->setColor(Color3B::GRAY);
        btnBlend->setPosition(pos);
        btnBlend->setTitleFontSize(fontSize);
        btnBlend->setTitleColor(Color3B::BLACK);
        btnBlend->setTitleText("Blend");
        BIND_LISTENER_TOUCH(btnBlend, this, ManualDisplay::onSetSpriteBlendFunc);
        addChild(btnBlend);
    }
}

void ManualDisplay::onSetSpriteColor(Ref *pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            Color3B color = dynamic_cast<Button *>(pSender)->getColor();
            Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
            CC_BREAK_IF(!node);
            Vector<Node *> vec = node->getChildren();
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                Node * child = dynamic_cast<Node *>(*it);
                if (child) {
                    child->setColor(color);
                }
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onSetSpriteFlip(Ref *pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            int tag = dynamic_cast<Button *>(pSender)->getTag();
            if (tag == 0) { // flip X
                Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
                CC_BREAK_IF(!node);
                Vector<Node *> vec = node->getChildren();
                switch (manualType) {
                    case ManualType::SPRITE: {
                        for (auto it = vec.begin(); it != vec.end(); ++it) {
                            Sprite * child = dynamic_cast<Sprite *>(*it);
                            if (child) {
                                child->setFlippedX(!child->isFlippedX());
                            }
                        }
                        break;
                    }
                    case ManualType::UIIMAGEVIEW: {
                        for (auto it = vec.begin(); it != vec.end(); ++it) {
                            ImageView * child = dynamic_cast<ImageView *>(*it);
                            if (child) {
                                child->setFlippedX(!child->isFlippedX());
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                
            } else { // flip Y
                Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
                CC_BREAK_IF(!node);
                Vector<Node *> vec = node->getChildren();
                switch (manualType) {
                    case ManualType::SPRITE: {
                        for (auto it = vec.begin(); it != vec.end(); ++it) {
                            Sprite * child = dynamic_cast<Sprite *>(*it);
                            if (child) {
                                child->setFlippedY(!child->isFlippedY());
                            }
                        }
                        break;
                    }
                    case ManualType::UIIMAGEVIEW: {
                        for (auto it = vec.begin(); it != vec.end(); ++it) {
                            ImageView * child = dynamic_cast<ImageView *>(*it);
                            if (child) {
                                child->setFlippedY(!child->isFlippedY());
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onSetSpriteRotate(Ref * pSender, TouchEventType type)
{
    CC_UNUSED_PARAM(pSender);
    
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
            CC_BREAK_IF(!node);
            Vector<Node *> vec = node->getChildren();
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                Node * child = dynamic_cast<Node *>(*it);
                if (child) {
                    float rotation = child->getRotation();
                    rotation += 10;
                    child->setRotation(rotation);
                }
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onSetSpriteVisible(Ref * pSender, TouchEventType type)
{
    CC_UNUSED_PARAM(pSender);

    switch (type) {
        case TOUCH_EVENT_ENDED: {
            Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
            CC_BREAK_IF(!node);
            Vector<Node *> vec = node->getChildren();
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                Node * child = dynamic_cast<Node *>(*it);
                if (child) {
                    child->setVisible(!child->isVisible());
                }
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onSetSpriteSkew(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            int tag = dynamic_cast<Button *>(pSender)->getTag();
            if (tag == 0) { // flip X
                Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
                CC_BREAK_IF(!node);
                Vector<Node *> vec = node->getChildren();
                for (auto it = vec.begin(); it != vec.end(); ++it) {
                    Node * child = dynamic_cast<Node *>(*it);
                    if (child) {
                        float sx = child->getSkewX();
                        sx += 5;
                        child->setSkewX(sx);
                    }
                }
                
            } else { // flip Y
                Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
                CC_BREAK_IF(!node);
                Vector<Node *> vec = node->getChildren();
                for (auto it = vec.begin(); it != vec.end(); ++it) {
                    Node * child = dynamic_cast<Node *>(*it);
                    if (child) {
                        float sy = child->getSkewY();
                        sy += 5;
                        child->setSkewY(sy);
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onSetSpriteScale(Ref * pSender, TouchEventType type)
{
    CC_UNUSED_PARAM(pSender);
    
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
            CC_BREAK_IF(!node);
            Vector<Node *> vec = node->getChildren();
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                Node * child = dynamic_cast<Node *>(*it);
                if (child) {
                    float scale = child->getScale();
                    scale -= 0.1;
                    if (scale < 0.5) {
                        scale = 1.5;
                    }
                    child->setScale(scale);
                }
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onSetSpriteOpacity(Ref * pSender, TouchEventType type)
{
    CC_UNUSED_PARAM(pSender);
    
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
            CC_BREAK_IF(!node);
            Vector<Node *> vec = node->getChildren();
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                Node * child = dynamic_cast<Node *>(*it);
                if (child) {
                    GLubyte opacity = child->getOpacity();
                    opacity -= 10;
                    if (opacity < 10) {
                        opacity = 255;
                    }
                    child->setOpacity(opacity);
                }
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onSetSpriteBlendFunc(Ref * pSender, TouchEventType type)
{
    CC_UNUSED_PARAM(pSender);
    
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            static BlendFunc bf[4] = {BlendFunc::DISABLE, BlendFunc::ALPHA_PREMULTIPLIED,
                BlendFunc::ALPHA_NON_PREMULTIPLIED, BlendFunc::ADDITIVE};
            static int currBlendFunc;
            if (++currBlendFunc == 4) {
                currBlendFunc = 0;
            }
            Node * node = dynamic_cast<Node *>(getChildByTag(Tag::TAG_NODESPRITE));
            CC_BREAK_IF(!node);
            Vector<Node *> vec = node->getChildren();
            switch (manualType) {
                case ManualType::SPRITE:
                    for (auto it = vec.begin(); it != vec.end(); ++it) {
                        Sprite * child = dynamic_cast<Sprite *>(*it);
                        if (child) {
                            child->setBlendFunc(bf[currBlendFunc]);
                        }
                    }
                    break;
                
                default:
                    break;
            }
            
            break;
        }
        default:
            break;
    }
}

#pragma mark -

void ManualDisplay::addActionController()
{
    Point pos = Point(40, 40);
    const float pad = 75;
    float fontSize = 14;
    
    // move
    auto btnMove = Button::create("white.png");
    btnMove->setColor(Color3B::GRAY);
    btnMove->setPosition(pos);
    btnMove->setTitleFontSize(fontSize);
    btnMove->setTitleColor(Color3B::BLACK);
    btnMove->setTitleText("Move");
    btnMove->setTag(0);
    BIND_LISTENER_TOUCH(btnMove, this, ManualDisplay::onActionMove);
    addChild(btnMove);
    
    pos.x += pad;
    
    // rotation
    auto btnRotate = Button::create("white.png");
    btnRotate->setColor(Color3B::GRAY);
    btnRotate->setPosition(pos);
    btnRotate->setTitleFontSize(fontSize);
    btnRotate->setTitleColor(Color3B::BLACK);
    btnRotate->setTitleText("Rotate");
    BIND_LISTENER_TOUCH(btnRotate, this, ManualDisplay::onActionRotate);
    addChild(btnRotate);
    
    pos.x += pad;
    
    // visible
    auto btnVisible = Button::create("white.png");
    btnVisible->setColor(Color3B::GRAY);
    btnVisible->setPosition(pos);
    btnVisible->setTitleFontSize(fontSize);
    btnVisible->setTitleColor(Color3B::BLACK);
    btnVisible->setTitleText("Visible");
    BIND_LISTENER_TOUCH(btnVisible, this, ManualDisplay::onActionVisible);
    addChild(btnVisible);
    
    pos.x += pad;
    
    // skew
    auto btnSkew = Button::create("white.png");
    btnSkew->setColor(Color3B::GRAY);
    btnSkew->setPosition(pos);
    btnSkew->setTitleFontSize(fontSize);
    btnSkew->setTitleColor(Color3B::BLACK);
    btnSkew->setTitleText("Skew");
    btnSkew->setTag(0);
    BIND_LISTENER_TOUCH(btnSkew, this, ManualDisplay::onActionSkew);
    addChild(btnSkew);
    
    pos.x += pad;
    
    // scale
    auto btnScale = Button::create("white.png");
    btnScale->setColor(Color3B::GRAY);
    btnScale->setPosition(pos);
    btnScale->setTitleFontSize(fontSize);
    btnScale->setTitleColor(Color3B::BLACK);
    btnScale->setTitleText("Scale");
    BIND_LISTENER_TOUCH(btnScale, this, ManualDisplay::onActionScale);
    addChild(btnScale);
    
    pos.x += pad;
    
    // opacity
    auto btnFade = Button::create("white.png");
    btnFade->setColor(Color3B::GRAY);
    btnFade->setPosition(pos);
    btnFade->setTitleFontSize(fontSize);
    btnFade->setTitleColor(Color3B::BLACK);
    btnFade->setTitleText("Fade");
    BIND_LISTENER_TOUCH(btnFade, this, ManualDisplay::onActionFade);
    addChild(btnFade);
    
    pos.x += pad;
    
    // Tint
    auto btnTint = Button::create("white.png");
    btnTint->setColor(Color3B::GRAY);
    btnTint->setPosition(pos);
    btnTint->setTitleFontSize(fontSize);
    btnTint->setTitleColor(Color3B::BLACK);
    btnTint->setTitleText("Tint");
    BIND_LISTENER_TOUCH(btnTint, this, ManualDisplay::onActionTint);
    addChild(btnTint);
    
    pos.x += pad;
    
    // jump
    auto btnJump = Button::create("white.png");
    btnJump->setColor(Color3B::GRAY);
    btnJump->setPosition(pos);
    btnJump->setTitleFontSize(fontSize);
    btnJump->setTitleColor(Color3B::BLACK);
    btnJump->setTitleText("Jump");
    BIND_LISTENER_TOUCH(btnJump, this, ManualDisplay::onActionJump);
    addChild(btnJump);
    
    pos.x += pad;
    
    // Bezier
    auto btnBezier = Button::create("white.png");
    btnBezier->setColor(Color3B::GRAY);
    btnBezier->setPosition(pos);
    btnBezier->setTitleFontSize(fontSize);
    btnBezier->setTitleColor(Color3B::BLACK);
    btnBezier->setTitleText("Bezier");
    BIND_LISTENER_TOUCH(btnBezier, this, ManualDisplay::onActionBezier);
    addChild(btnBezier);
    
    pos.x += pad;
    
    // Target
    auto btnTarget = Button::create("white.png");
    btnTarget->setColor(Color3B::GRAY);
    btnTarget->setPosition(pos);
    btnTarget->setTitleFontSize(fontSize);
    btnTarget->setTitleColor(Color3B::BLACK);
    btnTarget->setTitleText("Target");
    BIND_LISTENER_TOUCH(btnTarget, this, ManualDisplay::onActionTarget);
    addChild(btnTarget);

}

void ManualDisplay::onActionMove(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // MoveTo
            Point pos = sprMan->getPosition();
            MoveTo * moveTo = MoveTo::create(0.5, imvWolf->getPosition());
            MoveTo * moveBack = MoveTo::create(0.5, pos);
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::createWithTwoActions(moveTo, moveBack));
            // MoveBy
            MoveBy * moveBy = MoveBy::create(0.5, Point(0, -100));
            MoveBy * moveByBack = moveBy->reverse();
            imvWolf->stopAllActions();
            imvWolf->runAction(Sequence::createWithTwoActions(moveBy, moveByBack));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionRotate(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // RotateTo
            RotateTo * rotateTo = RotateTo::create(0.5, 180, -180);
            RotateTo * rotateBack = RotateTo::create(0.5, 0);
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::createWithTwoActions(rotateTo, rotateBack));
            // RotateBy
            RotateBy * rotateBy = RotateBy::create(0.5, -180);
            RotateBy * rotateByBack = rotateBy->reverse();
            imvWolf->stopAllActions();
            imvWolf->runAction(Sequence::createWithTwoActions(rotateBy, rotateByBack));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionVisible(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // hide -> delay -> show -> delay
            Sequence * action = Sequence::create(Hide::create(), DelayTime::create(0.5), Show::create(), DelayTime::create(0.5), NULL);
            sprMan->stopAllActions();
            sprMan->runAction(RepeatForever::create(action));
            // Blink
            Blink * blink = Blink::create(0.5, 10);
            imvWolf->stopAllActions();
            imvWolf->runAction(blink);
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionSkew(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // SkewTo
            SkewTo * skewTo = SkewTo::create(0.5, 15, 25);
            SkewTo * skewBack = SkewTo::create(0.5, 1, 1);
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::createWithTwoActions(skewTo, skewBack));
            // SkewBy
            SkewBy * skewBy = SkewBy::create(0.5, 25, 15);
            SkewBy * skewByBack = skewBy->reverse();
            imvWolf->stopAllActions();
            imvWolf->runAction(Sequence::createWithTwoActions(skewBy, skewByBack));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionScale(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // ScaleTo
            ScaleTo * scaleTo = ScaleTo::create(0.5, 1.5, 0.5);
            ScaleTo * scaleBack = ScaleTo::create(0.5, 1);
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::createWithTwoActions(scaleTo, scaleBack));
            // ScaleBy
            ScaleBy * scaleBy = ScaleBy::create(0.5, 1.5, 0.5);
            ScaleBy * scaleByBack = scaleBy->reverse();
            imvWolf->stopAllActions();
            imvWolf->runAction(Sequence::createWithTwoActions(scaleBy, scaleByBack));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionFade(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // Fade In & Out
            FadeIn * fadeIn = FadeIn::create(0.5);
            FadeOut * fadeOut = FadeOut::create(0.5);
            sprMan->stopAllActions();
            sprMan->runAction(RepeatForever::create(Sequence::createWithTwoActions(fadeIn, fadeOut)));
            // Fade In & Out
            FadeIn * fadeIn2 = FadeIn::create(0.8);
            FadeOut * fadeOut2 = FadeOut::create(0.8);
            imvWolf->stopAllActions();
            imvWolf->runAction(RepeatForever::create(Sequence::createWithTwoActions(fadeIn2, fadeOut2)));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionTint(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // TintTo
            TintTo * tintTo = TintTo::create(0.5, 100, 82, 183);
            TintTo * tintBack = TintTo::create(0.5, 200, 120, 30);
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::createWithTwoActions(tintTo, tintBack));
            // TintBy
            TintBy * tintBy = TintBy::create(0.8, 100, 123, 225);
            TintBy * tintByBack = tintBy->reverse();
            imvWolf->stopAllActions();
            imvWolf->runAction(Sequence::createWithTwoActions(tintBy, tintByBack));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionJump(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // JumpTo
            Point pos = sprMan->getPosition();
            JumpTo * jumpTo = JumpTo::create(5,  imvWolf->getPosition(), 200, 3);
            JumpTo * jumpBack = JumpTo::create(5, pos, 150, 4);
            Spawn * actionBack = Spawn::createWithTwoActions(jumpBack, RotateBy::create(5, -360 * 4));
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::createWithTwoActions(jumpTo, actionBack));
            // JumpBy
            JumpBy * jumpBy = JumpBy::create(3, Point(0, -100), 250, 2);
            JumpBy * jumpByBack = jumpBy->reverse();
            imvWolf->stopAllActions();
            imvWolf->runAction(Sequence::createWithTwoActions(jumpBy, jumpByBack));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionBezier(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // BezierTo
            Point pos = sprMan->getPosition();
            Point posWolf = imvWolf->getPosition();
            
            ccBezierConfig bzConfigTo, bzConfigBack;
            bzConfigTo.endPosition = posWolf;
            bzConfigTo.controlPoint_1 = pos + Point(100, 200);
            bzConfigTo.controlPoint_2 = posWolf - Point(100, 200);
            bzConfigBack.endPosition = pos;
            bzConfigBack.controlPoint_1 = posWolf - Point(150, -200);
            bzConfigBack.controlPoint_2 = pos + Point(150, -200);

            BezierTo * bzTo = BezierTo::create(5, bzConfigTo);
            BezierTo * bzBack = BezierTo::create(5, bzConfigBack);
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::createWithTwoActions(bzTo, bzBack));
            // BezierBy
            ccBezierConfig bzConfigBy;
            bzConfigBy.endPosition = posWolf;
            bzConfigBy.controlPoint_1 = Point(100, 100);
            bzConfigBy.controlPoint_2 = Point(-100, 200);
            BezierBy * bzBy = BezierBy::create(3, bzConfigBy);
            BezierBy * bzByBack = bzBy->reverse();
            imvWolf->stopAllActions();
            imvWolf->runAction(Sequence::createWithTwoActions(bzBy, bzByBack));
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onActionTarget(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            auto sprMan = getChildByTag(Tag::TAG_ACTOR_MAN);
            auto imvWolf = getChildByTag(Tag::TAG_ACTOR_WOLF);
            CC_BREAK_IF(!sprMan || !imvWolf);
            // Move
            Point pos = sprMan->getPosition();
            MoveTo * moveTo = MoveTo::create(0.5, imvWolf->getPosition());
            EaseBackInOut * easeMove = EaseBackInOut::create(moveTo);
            MoveTo * moveBack = MoveTo::create(0.5, pos);
            EaseElasticInOut * easeBack = EaseElasticInOut::create(moveBack, 1);
            // Target Jump
            JumpBy * jumpBy = JumpBy::create(2, Point(0, -100), 250, 1);
            JumpBy * jumpByBack = jumpBy->reverse();
            imvWolf->stopAllActions();
            TargetedAction * target = TargetedAction::create(imvWolf, Sequence::createWithTwoActions(jumpBy, jumpByBack));
            sprMan->stopAllActions();
            sprMan->runAction(Sequence::create(easeMove, target, easeBack, NULL));
            break;
        }
        default:
            break;
    }
}

#pragma mark -

void ManualDisplay::onTouchRichText(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            Button * btn = dynamic_cast<Button *>(pSender);
            CC_BREAK_IF(!btn);
            string tip = StringUtils::format("Touch - %s", btn->getTitleText().c_str());
            auto rxShow = getChildByTag(Tag::TAG_SELECTSHOW);
            CC_BREAK_IF(!rxShow);
            dynamic_cast<Label *>(rxShow)->setString(tip);
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onTouchButton(Ref * pSender, TouchEventType type)
{
    switch (type) {
        case TOUCH_EVENT_ENDED: {
            Button * btn = dynamic_cast<Button *>(pSender);
            CC_BREAK_IF(!btn);
            if (btn->getTag() == 0) {
                Button * btnCon = dynamic_cast<Button *>(getChildByTag(1));
                WIDGET_ENABEL(btnCon, true);
            }
            auto labTitle = getChildByTag(Tag::TAG_BUTTONTITLE);
            CC_BREAK_IF(!labTitle);
            dynamic_cast<Label *>(labTitle)->setString(btn->getName());
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onTouchCheckBox(Ref * pSender, CheckBoxEventType type)
{
    switch (type) {
        case CHECKBOX_STATE_EVENT_SELECTED: {
            string cbName = dynamic_cast<CheckBox *>(pSender)->getName();
            Label * labShow = dynamic_cast<Label *>(getChildByTag(Tag::TAG_SELECTSHOW));
            CC_BREAK_IF(!labShow);
            labShow->setString(StringUtils::format("%s Selected", cbName.c_str()));
            CheckBoxExt * checkBox = nullptr;
            if (cbName.compare("Tick CheckBox") == 0) {
                checkBox = dynamic_cast<CheckBoxExt *>(getChildByTag(Tag::TAG_CHECKBOXTICK));
            } else if (cbName.compare("Star CheckBox") == 0) {
                checkBox = dynamic_cast<CheckBoxExt *>(getChildByTag(Tag::TAG_CHECKBOXSTAR));
            }
            CC_BREAK_IF(!checkBox);
            WIDGET_ENABEL(checkBox, true);
            break;
        }
        case CHECKBOX_STATE_EVENT_UNSELECTED: {
            string cbName = dynamic_cast<CheckBox *>(pSender)->getName();
            Label * labShow = dynamic_cast<Label *>(getChildByTag(Tag::TAG_SELECTSHOW));
            CC_BREAK_IF(!labShow);
            labShow->setString(StringUtils::format("%s Unselected", cbName.c_str()));
            CheckBoxExt * checkBox = nullptr;
            if (cbName.compare("Tick CheckBox") == 0) {
                checkBox = dynamic_cast<CheckBoxExt *>(getChildByTag(Tag::TAG_CHECKBOXTICK));
            } else if (cbName.compare("Star CheckBox") == 0) {
                checkBox = dynamic_cast<CheckBoxExt *>(getChildByTag(Tag::TAG_CHECKBOXSTAR));
            }
            CC_BREAK_IF(!checkBox);
            WIDGET_ENABEL(checkBox, false);
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onTouchSlider(Ref * pSender, SliderEventType type)
{
    switch (type) {
        case SLIDER_PERCENTCHANGED: {
            Slider * slider = dynamic_cast<Slider *>(pSender);
            string tipShow = StringUtils::format("%s: %i", slider->getName(), slider->getPercent());
            Label * labShow = dynamic_cast<Label *>(getChildByTag(Tag::TAG_SELECTSHOW));
            CC_BREAK_IF(!labShow);
            labShow->setString(tipShow);
            
            if (slider->getTag() == Tag::TAG_SLIDERALL) {
                Slider * sliderL = dynamic_cast<Slider *>(getChildByTag(Tag::TAG_SLIDERLEFT));
                Slider * sliderR = dynamic_cast<Slider *>(getChildByTag(Tag::TAG_SLIDERRIGHT));
                sliderL->setPercent(slider->getPercent());
                sliderR->setPercent(slider->getPercent());
            }
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onTouchScollView(Ref * pSender, ScrollviewEventType type)
{
    switch (type) {
        case ui::SCROLLVIEW_EVENT_SCROLLING: {
            ui::ScrollView * sv = dynamic_cast<ui::ScrollView *>(pSender);
            CC_BREAK_IF(!sv);
            auto pos = sv->getInnerContainer()->getPosition();
            string strPos = StringUtilsExt::stringFromPoint(pos);
            string tipShow = StringUtils::format("Scroll to: %s", strPos.c_str());
            Label * labShow = dynamic_cast<Label *>(getChildByTag(Tag::TAG_SELECTSHOW));
            CC_BREAK_IF(!labShow);
            labShow->setString(tipShow);
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onTouchListView(Ref * pSender, ListViewEventType type)
{
    switch (type) {
        case LISTVIEW_ONSELECTEDITEM_END: {
            ListView * list = dynamic_cast<ListView *>(pSender);
            CC_BREAK_IF(!list);
            Text * item = dynamic_cast<Text *>(list->getItem(list->getCurSelectedIndex()));
            CC_BREAK_IF(!item);
            string tipShow = StringUtils::format("Select: %s", item->getStringValue().c_str());
            Label * labShow = dynamic_cast<Label *>(getChildByTag(Tag::TAG_SELECTSHOW));
            CC_BREAK_IF(!labShow);
            labShow->setString(tipShow);
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onTouchPageView(Ref * pSender, PageViewEventType type)
{
    switch (type) {
        case PAGEVIEW_EVENT_TURNING: {
            PageView * pv = dynamic_cast<PageView *>(pSender);
            CC_BREAK_IF(!pv);
            string tipShow = StringUtils::format("Select: %zi", pv->getCurPageIndex());
            Label * labShow = dynamic_cast<Label *>(getChildByTag(Tag::TAG_SELECTSHOW));
            CC_BREAK_IF(!labShow);
            labShow->setString(tipShow);
            break;
        }
        default:
            break;
    }
}

void ManualDisplay::onHttpRequstCallback(HttpClient * client, HttpResponse * response)
{
    string ret("");
    if (!response || !response->isSucceed()) {
        ret = "response fail";
        return;
    } else {
        vector<char> * buffer = response->getResponseData();
        string responseStr(buffer->begin(), buffer->end());
        responseStr = replace_all(responseStr, "\n", "");
        responseStr = replace_all(responseStr, "\r", "");
        ret = responseStr;
    }
    
    Label * labRet = dynamic_cast<Label *>(getChildByTag(Tag::TAG_SELECTSHOW));
    if (labRet) {
        labRet->setString(ret);
    }
}

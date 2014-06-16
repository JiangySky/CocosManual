//
//  UIExtension.cpp
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#include "UIExtension.h"
#include "cocos-ext.h"

NS_CC_BEGIN

namespace ui {
    
#pragma mark - ButtonText
    
    ButtonText * ButtonText::create(const std::string& text, const Color3B& color,
                                    const std::string& fontName/* = "Helvetica"*/, float size/* = 20*/)
    {
        ButtonText * btn = new ButtonText();
        if (btn && btn->init(text, color, fontName, size)) {
            btn->autorelease();
            return btn;
        }
        CC_SAFE_DELETE(btn);
        return nullptr;
    }
    
    bool ButtonText::init(const std::string &text, const cocos2d::Color3B &color, const std::string &fontName, float size)
    {
        bool ret = true;
        do {
            if (!Button::init()) {
                ret = false;
                break;
            }
            
            setTouchEnabled(true);
            setTitleText(text);
            setTitleColor(color);
            setTitleFontName(fontName);
            setTitleFontSize(size);
            ignoreContentAdaptWithSize(false);
            setSize(_titleRenderer->getContentSize());
            
        } while (0);
        return ret;
    }
    
#pragma mark - ImageViewExt
    
    ImageViewExt * ImageViewExt::create(const std::string &imageFileName, const cocos2d::Rect &rect, SizeType sizeType)
    {
        ImageViewExt * imv = new ImageViewExt();
        if (imv && imv->init(imageFileName, rect, sizeType)) {
            imv->autorelease();
            return imv;
        }
        CC_SAFE_DELETE(imv);
        return nullptr;
    }
    
    bool ImageViewExt::init(const std::string &imageFileName, const cocos2d::Rect &rect, SizeType sizeType)
    {
        bool bRet = true;
        do {
            if (!ImageView::init(imageFileName)) {
                bRet = false;
                break;
            }
            setTextureRect(rect, sizeType);
        } while (0);
        return bRet;
    }
    
    void ImageViewExt::setTextureRect(const cocos2d::Rect &rect, SizeType sizeType)
    {
        Rect rectText = Rect(rect);
        if (sizeType == SIZE_PERCENT) {
            Size size = getContentSize();
            rectText.origin.x *= size.width;
            rectText.origin.y *= size.height;
            rectText.size.width *= size.width;
            rectText.size.height *= size.height;
        }
        ImageView::setTextureRect(rectText);
    }
    
#pragma mark - CheckBoxExt
    
    CheckBoxExt::CheckBoxExt() :
    _titleRenderer(nullptr),
    _titleVisible(true),
    _titleEnable(true),
    _titleNormalColor(Color3B::BLACK),
    _titleDisabledColor(Color3B::GRAY)
    {
    }
    CheckBoxExt::~CheckBoxExt()
    {
        
    }
    
    CheckBoxExt * CheckBoxExt::create(CheckBox * checkBox, Label * label/* = nullptr*/)
    {
        CheckBoxExt* widget = new CheckBoxExt();
        if (widget && widget->init())
        {
            widget->autorelease();
            widget->copyProperties(checkBox);
            widget->initWithSize();
            widget->setTitle(label, true);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    CheckBoxExt * CheckBoxExt::create(const std::string& bg, const std::string& cross, TextureResType texType,
                                      const std::string& fontName, const Color3B& fontColor, float fontSize,
                                      const std::string& title)
    {
        CheckBoxExt* widget = new CheckBoxExt();
        if (widget && widget->init())
        {
            widget->autorelease();
            widget->loadTextures(bg, bg, cross, bg, cross, texType);
            widget->initWithSize();
            widget->setTitle(fontName, fontColor, fontSize);
            widget->setTitleText(title);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    void CheckBoxExt::initRenderer()
    {
        CheckBox::initRenderer();
        _titleRenderer = Label::createWithSystemFont("", "Helvetica", 30);
        _titleRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        addProtectedChild(_titleRenderer, -1, -1);
    }
    
    void CheckBoxExt::initWithSize()
    {
        Size size = getSize();
        _titleRenderer->setPosition(Point(size.width + 10, size.height / 2));
    }
    
    void CheckBoxExt::setTitle(Label * label, bool defaultWhenNull/* = false*/)
    {
        removeChildByTag(CheckBoxExt::Tag::TAG_CHECKBOX_TITLE);
        if (label) {
            label->setTag(CheckBoxExt::Tag::TAG_CHECKBOX_TITLE);
            label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            label->setPosition(_titleRenderer->getPosition());
            _titleRenderer->setVisible(false);
            _titleNormalColor = label->getColor();
            if (label->getParent()) {
                label->removeFromParent();
            }
            addChild(label);
        } else if (defaultWhenNull) {
            setTitle("Helvetica", Color3B::BLACK, 30);
        }
    }
    
    void CheckBoxExt::setTitle(const std::string& fontName, const Color3B& fontColor, float fontSize,
                               Color3B disableColor/* = Color3B::GRAY*/, bool shadow/* = true*/)
    {
        Label * label = static_cast<Label *>(_titleRenderer);
        label->setSystemFontName(fontName);
        label->setColor(fontColor);
        label->setSystemFontSize(fontSize);
        if (shadow) {
            label->enableShadow();
        }
        setTitleColor(fontColor);
    }
    
    void CheckBoxExt::setTitleText(const std::string& title)
    {
        if (_titleVisible) {
            static_cast<Label *>(_titleRenderer)->setString(title);
        } else {
            auto label = getChildByTag(CheckBoxExt::Tag::TAG_CHECKBOX_TITLE);
            if (label) {
                dynamic_cast<Label *>(label)->setString(title);
            }
        }
    }
    
    void CheckBoxExt::setTitleColor(Color3B color)
    {
        _titleNormalColor = color;
    }
    
    void CheckBoxExt::setTitleDisabledColor(Color3B color)
    {
        _titleDisabledColor = color;
    }
    
    void CheckBoxExt::setTitlePosition(const Point& pos, SizeType sizeType)
    {
        Point posAbsolute = Point(pos);
        if (sizeType == SIZE_PERCENT) {
            Size size = getSize();
            posAbsolute.x *= size.width;
            posAbsolute.y *= size.height;
        }
        _titleRenderer->setPosition(posAbsolute);
        
        auto label = getChildByTag(CheckBoxExt::Tag::TAG_CHECKBOX_TITLE);
        if (label) {
            label->setPosition(posAbsolute);
        }
    }
    
    void CheckBoxExt::setTitleVisible(bool visible)
    {
        if (_titleVisible != visible) {
            _titleVisible = visible;
            _titleRenderer->setVisible(_titleVisible);
            
            auto label = getChildByTag(CheckBoxExt::Tag::TAG_CHECKBOX_TITLE);
            if (label) {
                label->setVisible(_titleVisible);
                _titleRenderer->setVisible(false);
            }
        }
    }
    
    void CheckBoxExt::setTitleEnable(bool enable)
    {
        if (_titleEnable != enable) {
            _titleEnable = enable;
            
            Color3B color = (enable ? _titleNormalColor : _titleDisabledColor);
            _titleRenderer->setColor(color);
            
            auto label = getChildByTag(CheckBoxExt::Tag::TAG_CHECKBOX_TITLE);
            if (label) {
                label->setColor(color);
            }
        }
    }
    
    void CheckBoxExt::onPressStateChangedToNormal()
    {
        CheckBox::onPressStateChangedToNormal();
        setTitleEnable(true);
    }
    
    void CheckBoxExt::onPressStateChangedToDisabled()
    {
        CheckBox::onPressStateChangedToDisabled();
        setTitleEnable(false);
    }
    
    RadioGroup::RadioGroup() :
    _currItem(-1),
    _itemAmount(0),
    _itemPad(5),
    _selectedChanged(nullptr)
    {
    }
    
    RadioGroup::~RadioGroup()
    {
        
    }
    
    RadioGroup * RadioGroup::create()
    {
        RadioGroup* widget = new RadioGroup();
        if (widget && widget->init())
        {
            widget->autorelease();
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    RadioGroup * RadioGroup::create(CheckBox * checkBox, Vector<Label *> labels, const std::function<void(Ref *)> & callBack)
    {
        RadioGroup* widget = new RadioGroup();
        if (widget && widget->init(checkBox, labels, callBack))
        {
            widget->autorelease();
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    RadioGroup * RadioGroup::create(const std::string& bg, const std::string& cross, TextureResType texType,
                                    const std::string& fontName, const Color3B& fontColor, float fontSize, int amount,
                                    const std::string title[], const std::function<void(Ref *)> & callBack)
    {
        RadioGroup* widget = new RadioGroup();
        if (widget && widget->init(bg, cross, texType, fontName, fontColor, fontSize, amount, title, callBack))
        {
            widget->autorelease();
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    bool RadioGroup::init()
    {
        bool bRet = true;
        bRet = Widget::init();
        setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        setSelectIndex(0);
        return bRet;
    }
    
    bool RadioGroup::init(CheckBox * checkBox, Vector<Label *> labels, const std::function<void(Ref *)> & callBack)
    {
        bool bRet = true;

        Point pos = Point::ZERO;
        _itemAmount = labels.size();
        for (int i = 0; i < _itemAmount; i++) {
            auto label = labels.at(i);
            CheckBoxExt * cbExt = CheckBoxExt::create(checkBox, label);
            CC_BREAK_IF(!cbExt);
            cbExt->setTag(i);
            cbExt->setName(label->getString().c_str());
            cbExt->setSelectedState(false);
            cbExt->addEventListenerCheckBox(this, checkboxselectedeventselector(RadioGroup::onCheckBoxChanged));
            cbExt->setPosition(pos);
            addChild(cbExt);
            
            pos.y -= _itemPad + cbExt->getContentSize().height;
        }
        
        setSelectedChangedCallBack(callBack);
        setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        setSelectIndex(0);
        
        return bRet;
    }
    
    bool RadioGroup::init(const std::string& bg, const std::string& cross, TextureResType texType,
              const std::string& fontName, const Color3B& fontColor, float fontSize, int amount,
              const std::string title[], const std::function<void(Ref *)> & callBack)
    {
        bool bRet = true;
        
        Point pos = Point::ZERO;
        _itemAmount = amount;
        for (int i = 0; i < _itemAmount; i++) {
            auto labTitle = title[i];
            CheckBoxExt * cbExt = CheckBoxExt::create(bg, cross, texType, fontName, fontColor, fontSize, labTitle);
            cbExt->setTag(i);
            cbExt->setName(labTitle.c_str());
            cbExt->setSelectedState(false);
            cbExt->addEventListenerCheckBox(this, checkboxselectedeventselector(RadioGroup::onCheckBoxChanged));
            cbExt->setPosition(pos);
            addChild(cbExt);
            
            pos.y -= _itemPad + cbExt->getContentSize().height;
        }
        
        setSelectedChangedCallBack(callBack);
        setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        setSelectIndex(0);
        
        return bRet;
    }
    
    void RadioGroup::setItemPad(float pad/* = 5*/)
    {
        if (pad != _itemPad) {
            _itemPad = pad;
            Point pos = Point::ZERO;
            for (int i = 0; i < _itemAmount; i++) {
                auto cbExt = getChildByTag(i);
                cbExt->setPosition(pos);
                pos.y -= _itemPad + cbExt->getContentSize().height;
            }
        }
    }
    
    void RadioGroup::setSelectedChangedCallBack(const std::function<void (Ref *)> &callBack)
    {
        _selectedChanged = callBack;
    }
    
    void RadioGroup::setSelectIndex(int index)
    {
        if (_currItem != index) {
            if (_currItem >= 0 && _currItem < _itemAmount) {
                CheckBoxExt * cbExt = dynamic_cast<CheckBoxExt *>(getChildByTag(_currItem));
                if (cbExt) {
                    cbExt->setSelectedState(false);
                    cbExt->setTouchEnabled(true);
                }
            }
            _currItem = index;
            if (_currItem >= 0 && _currItem < _itemAmount) {
                CheckBoxExt * cbExt = dynamic_cast<CheckBoxExt *>(getChildByTag(_currItem));
                if (cbExt) {
                    cbExt->setSelectedState(true);
                    cbExt->setTouchEnabled(false);
                }
            }
            
            if (_selectedChanged) {
                _selectedChanged(this);
            }
        }
    }
    
    void RadioGroup::onCheckBoxChanged(Ref * pSender, CheckBoxEventType type)
    {
        switch (type) {
            case CHECKBOX_STATE_EVENT_SELECTED: {
                CC_BREAK_IF(!pSender);
                setSelectIndex(dynamic_cast<CheckBoxExt *>(pSender)->getTag());
                break;
            }
            default:
                break;
        }
    }
    
    int RadioGroup::getSelectIndex()
    {
        return _currItem;
    }
    
    CheckBoxExt * RadioGroup::getSelectItem()
    {
        return dynamic_cast<CheckBoxExt *>(getChildByTag(_currItem));
    }
    
#pragma mark - LoadingBarExt
    
    LoadingBarExt::LoadingBarExt():
    _barBgRenderer(nullptr),
    _labPrgRenderer(nullptr),
    _renderBarBgTexType(UI_TEX_TYPE_LOCAL),
    _prgVisible(false),
    _textureBgFile(""),
    _callBackOnlyFinish(true),
    _loadingCallBack(nullptr)
    {
    }

    
    LoadingBarExt::~LoadingBarExt()
    {
        
    }
    
    LoadingBarExt * LoadingBarExt::create(LoadingBar * bar, Label * label/* = nullptr*/)
    {
        LoadingBarExt* widget = new LoadingBarExt();
        if (bar && widget && widget->init()) {
            widget->autorelease();
            widget->copyProperties(bar);
            widget->initWithSize();
            widget->setLabelProgress(label, true);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    LoadingBarExt * LoadingBarExt::create(const std::string &bg, const std::string &fg,
                                          TextureResType texType/* = UI_TEX_TYPE_LOCAL*/, int percentage/* = 0*/)
    {
        LoadingBarExt* widget = new LoadingBarExt();
        if (widget && widget->init()) {
            widget->autorelease();
            widget->loadTexture(fg, texType);
            widget->loadBgTexture(bg, texType);
            widget->initWithSize();
            widget->setPercent(percentage);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    LoadingBarExt * LoadingBarExt::create(const std::string& bg, const std::string& fg, TextureResType texType, int percentage,
                                          const std::string& fontName, const Color3B& fontColor, float fontSize)
    {
        LoadingBarExt* widget = new LoadingBarExt();
        if (widget && widget->init()) {
            widget->autorelease();
            widget->loadTexture(fg, texType);
            widget->loadBgTexture(bg, texType);
            widget->initWithSize();
            widget->setLabelProgress(fontName, fontColor, fontSize);
            widget->setLabelProgressVisible(true);
            widget->setPercent(percentage);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    void LoadingBarExt::initRenderer()
    {
        _barBgRenderer = Sprite::create();
        addProtectedChild(_barBgRenderer, -2, -1);
        
        LoadingBar::initRenderer();
        
        _labPrgRenderer = Label::create();
        addProtectedChild(_labPrgRenderer, -1, -1);
        setLabelProgressVisible(false);
    }
    
    void LoadingBarExt::initWithSize()
    {
        Size size = getSize();
        _barBgRenderer->setPosition(Point(size) / 2);
        setLabelProgressPosition(Point(size) / 2, SIZE_ABSOLUTE);
    }
    
    void LoadingBarExt::loadBgTexture(const std::string &texture, TextureResType texType/* = UI_TEX_TYPE_LOCAL*/)
    {
        if (texture.empty())
        {
            return;
        }
        _renderBarBgTexType = texType;
        _textureBgFile = texture;
        switch (_renderBarBgTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                if (_scale9Enabled)
                {
                    extension::Scale9Sprite* barRendererScale9 = static_cast<extension::Scale9Sprite*>(_barBgRenderer);
                    barRendererScale9->initWithFile(texture);
                    barRendererScale9->setCapInsets(_capInsets);
                }
                else
                {
                    static_cast<Sprite*>(_barBgRenderer)->setTexture(texture);
                }
                break;
            case UI_TEX_TYPE_PLIST:
                if (_scale9Enabled)
                {
                    extension::Scale9Sprite* barRendererScale9 = static_cast<extension::Scale9Sprite*>(_barBgRenderer);
                    barRendererScale9->initWithSpriteFrameName(texture);
                    barRendererScale9->setCapInsets(_capInsets);
                }
                else
                {
                    static_cast<Sprite*>(_barBgRenderer)->setSpriteFrame(texture);
                }
                break;
            default:
                break;
        }
        updateRGBAToRenderer(_barBgRenderer);
    }
    
    void LoadingBarExt::setLabelProgress(Label * label, bool defaultWhenNull/* = false*/)
    {
        removeChildByTag(LoadingBarExt::Tag::TAG_LOADINGBAR_LABEL);
        if (label) {
            label->setTag(LoadingBarExt::Tag::TAG_LOADINGBAR_LABEL);
            label->setPosition(_labPrgRenderer->getPosition());
            _labPrgRenderer->setVisible(false);
            if (label->getParent()) {
                label->removeFromParent();
            }
            addChild(label);
        } else if (defaultWhenNull) {
            setLabelProgress("Helvetica", Color3B::RED, 30, true);
        }
    }
    
    void LoadingBarExt::setLabelProgress(const std::string& fontName, const Color3B& fontColor, float fontSize, bool shadow/* = true*/)
    {
        Label * labPrg = static_cast<Label *>(_labPrgRenderer);
        labPrg->setSystemFontName(fontName);
        labPrg->setColor(fontColor);
        labPrg->setSystemFontSize(fontSize);
        if (shadow) {
            labPrg->enableShadow();
        }
    }
    
    void LoadingBarExt::setLabelProgressPosition(const Point& pos, SizeType sizeType)
    {
        Point posAbsolute = Point(pos);
        if (sizeType == SIZE_PERCENT) {
            Size size = getSize();
            posAbsolute.x *= size.width;
            posAbsolute.y *= size.height;
        }
        _labPrgRenderer->setPosition(posAbsolute);
        
        auto label = getChildByTag(LoadingBarExt::Tag::TAG_LOADINGBAR_LABEL);
        if (label) {
            label->setPosition(posAbsolute);
        }
    }
    
    void LoadingBarExt::setLabelProgressVisible(bool visible)
    {
        if (_prgVisible != visible) {
            _prgVisible = visible;
            _labPrgRenderer->setVisible(_prgVisible);
            
            auto label = getChildByTag(LoadingBarExt::Tag::TAG_LOADINGBAR_LABEL);
            if (label) {
                label->setVisible(_prgVisible);
                _labPrgRenderer->setVisible(false);
            }
        }
    }
    
    void LoadingBarExt::updateTextureColor()
    {
        LoadingBar::updateTextureColor();
        updateOpacityToRenderer(_barBgRenderer);
        updateOpacityToRenderer(_labPrgRenderer);
    }
    
    void LoadingBarExt::updateTextureOpacity()
    {
        LoadingBar::updateTextureOpacity();
        updateOpacityToRenderer(_barBgRenderer);
        updateOpacityToRenderer(_labPrgRenderer);
    }
    
    void LoadingBarExt::updateTextureRGBA()
    {
        LoadingBar::updateTextureRGBA();
        updateRGBAToRenderer(_barBgRenderer);
        updateRGBAToRenderer(_labPrgRenderer);
    }

    void LoadingBarExt::setCallBack(const std::function<void (Ref *)> & callBack, bool onlyFinish/* = true*/)
    {
        _callBackOnlyFinish = onlyFinish;
        _loadingCallBack = callBack;
    }
    
    void LoadingBarExt::setPercent(int percentage)
    {
        LoadingBar::setPercent(percentage);
        if (_prgVisible) {
            std::string progress = StringUtils::format("%i%%", percentage);
            auto subLabel = getChildByTag(LoadingBarExt::Tag::TAG_LOADINGBAR_LABEL);
            if (subLabel) {
                dynamic_cast<Label *>(subLabel)->setString(progress);
            } else {
                static_cast<Label *>(_labPrgRenderer)->setString(progress);
            }
        }
        if (_loadingCallBack && (!_callBackOnlyFinish || (_callBackOnlyFinish && percentage == 100))) {
            _loadingCallBack(this);
        }
    }

#pragma mark - SliderExt
    
    SliderFine::SliderFine() :
    _sliderTextFieldRenderer(nullptr),
    _textFieldVisible(true),
    _textFieldEnable(true)
    {
    }
    
    SliderFine::~SliderFine()
    {
        
    }
    
    SliderFine * SliderFine::create(Slider * slider, TextField * textField/* = nullptr*/)
    {
        SliderFine * widget = new SliderFine();
        if (slider && widget && widget->init())
        {
            widget->autorelease();
            widget->copyProperties(slider);
            widget->initWithSize();
            widget->setTextField(textField, true);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    SliderFine * SliderFine::create(const std::string& barName, const std::string& ballName,
                                    const std::string& progressName/* = ""*/, TextureResType texType/* = UI_TEX_TYPE_LOCAL*/)
    {
        SliderFine * widget = new SliderFine();
        if (widget && widget->init())
        {
            widget->autorelease();
            widget->loadBarTexture(barName, texType);
            widget->loadSlidBallTextures(ballName, ballName, ballName, texType);
            widget->loadProgressBarTexture(progressName, texType);
            widget->initWithSize();
            widget->setTextField(nullptr, true);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    SliderFine * SliderFine::create(const std::string& barName, const std::string& ballName, const std::string& progressName,
                                    TextureResType texType, const std::string& fontName, Color3B fontColor, float fontSize)
    {
        SliderFine * widget = new SliderFine();
        if (widget && widget->init())
        {
            widget->autorelease();
            widget->loadBarTexture(barName, texType);
            widget->loadSlidBallTextures(ballName, ballName, ballName, texType);
            widget->loadProgressBarTexture(progressName, texType);
            widget->initWithSize();
            widget->setTextField(fontName, fontColor, fontSize);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    void SliderFine::initRenderer()
    {
        Slider::initRenderer();
        _sliderTextFieldRenderer = TextField::create("0", "Helvetica", 30);
        _sliderTextFieldRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        static_cast<TextField *>(_sliderTextFieldRenderer)->setMaxLength(3);
        static_cast<TextField *>(_sliderTextFieldRenderer)->addEventListenerTextField(this, textfieldeventselector(SliderFine::onTextFieldChanged));
        addProtectedChild(_sliderTextFieldRenderer, -1, -1);
    }
    
    
    bool SliderFine::onTouchBegan(Touch *touch, Event *unusedEvent)
    {
        bool pass = Widget::onTouchBegan(touch, unusedEvent);
        if (_hitted)
        {
            Point nsp = convertToNodeSpace(_touchStartPos);
            setPercent(getPercentWithBallPos(nsp.x));
            percentChangedEvent();
        }
        return pass;
    }
    
    void SliderFine::onTouchMoved(Touch *touch, Event *unusedEvent)
    {
        _touchMovePos = touch->getLocation();
        Point nsp = convertToNodeSpace(_touchMovePos);
        setPercent(getPercentWithBallPos(nsp.x));
        percentChangedEvent();
    }

    void SliderFine::initWithSize()
    {
        Size size = getSize();
        float posX = size.width + _slidBallDisabledRenderer->getContentSize().width / 2 + 10;
        setTextFieldPosition(Point(posX, size.height / 2), SIZE_ABSOLUTE);
    }
    
    void SliderFine::setTextField(TextField * textField, bool defaultWhenNull/* = false*/)
    {
        removeChildByTag(SliderFine::Tag::TAG_SLIDER_TEXTFIELD);
        if (textField) {
            textField->setTag(SliderFine::Tag::TAG_SLIDER_TEXTFIELD);
            textField->setPosition(_sliderTextFieldRenderer->getPosition());
            textField->setMaxLength(3);
            textField->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            textField->addEventListenerTextField(this, textfieldeventselector(SliderFine::onTextFieldChanged));
            _sliderTextFieldRenderer->setVisible(false);
            if (textField->getParent()) {
                textField->removeFromParent();
            }
            addChild(textField);
        } else if (defaultWhenNull) {
            setTextField("Helvetica", Color3B::RED, 30);
        }
    }
    
    void SliderFine::setTextField(const std::string& fontName, Color3B fontColor, float fontSize)
    {
        TextField * textField = static_cast<TextField *>(_sliderTextFieldRenderer);
        textField->setFontName(fontName);
        textField->setColor(fontColor);
        textField->setFontSize(fontSize);
    }

    void SliderFine::setTextFieldPosition(const Point& pos, SizeType sizeType)
    {
        Point posAbsolute = Point(pos);
        if (sizeType == SIZE_PERCENT) {
            Size size = getSize();
            posAbsolute.x *= size.width;
            posAbsolute.y *= size.height;
            if (pos.x <= 0) {
                _sliderTextFieldRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
            } else if (pos.x >= 1) {
                _sliderTextFieldRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            } else {
                _sliderTextFieldRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE);
            }
        }
        _sliderTextFieldRenderer->setPosition(posAbsolute);
        
        auto textField = getChildByTag(SliderFine::Tag::TAG_SLIDER_TEXTFIELD);
        if (textField) {
            textField->setPosition(posAbsolute);
            if (sizeType == SIZE_PERCENT) {
                if (pos.x <= 0) {
                    _sliderTextFieldRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
                } else if (pos.x >= 1) {
                    _sliderTextFieldRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
                } else {
                    _sliderTextFieldRenderer->setAnchorPoint(Point::ANCHOR_MIDDLE);
                }
            }
        }
    }

    void SliderFine::setTextFieldVisible(bool visible)
    {
        if (_textFieldVisible != visible) {
            _textFieldVisible = visible;
            _sliderTextFieldRenderer->setVisible(_textFieldVisible);
            
            auto textField = getChildByTag(SliderFine::Tag::TAG_SLIDER_TEXTFIELD);
            if (textField) {
                textField->setVisible(_textFieldVisible);
                _sliderTextFieldRenderer->setVisible(false);
            }
        }
    }

    void SliderFine::setTextFieldTouchEnable(bool enable)
    {
        if (_textFieldEnable != enable) {
            _textFieldEnable = enable;
            _sliderTextFieldRenderer->setVisible(_textFieldVisible);
            static_cast<TextField *>(_sliderTextFieldRenderer)->setTouchEnabled(_textFieldEnable);
            
            auto textField = getChildByTag(SliderFine::Tag::TAG_SLIDER_TEXTFIELD);
            if (textField) {
                dynamic_cast<TextField *>(textField)->setTouchEnabled(_textFieldEnable);
            }
        }
    }
        
    void SliderFine::setPercent(int percentage, bool manual/* = false*/)
    {
        Slider::setPercent(percentage);
        if (_textFieldVisible) {
            std::string percent = StringUtils::format("%i", getPercent());
            auto textField = getChildByTag(LoadingBarExt::Tag::TAG_LOADINGBAR_LABEL);
            if (textField) {
                dynamic_cast<TextField *>(textField)->setText(percent);
            } else {
                static_cast<TextField *>(_sliderTextFieldRenderer)->setText(percent);
            }
        }
    }
    
    void SliderFine::onTextFieldChanged(cocos2d::Ref *pSender, TextFiledEventType type)
    {
        switch (type) {
            case TEXTFIELD_EVENT_INSERT_TEXT:
            case TEXTFIELD_EVENT_DELETE_BACKWARD: {
                TextField * textField = static_cast<TextField *>(pSender);
                CC_BREAK_IF(!textField);
                std::string content = textField->getStringValue();
                if (content.length() > 0) {
                    const char * c = content.substr(content.length() - 1).c_str();
                    if (!isprint((int)(c[0])) || (content.length() == 1 && c[0] == '0')) {
                        content = "";
                    } else if (!isdigit((int)(c[0])) || content.length() > textField->getMaxLength()) {
                        content = content.substr(0, content.length() - 1);
                    }
                }
                textField->setText(content);
                break;
            }
            case TEXTFIELD_EVENT_DETACH_WITH_IME: {
                TextField * textField = static_cast<TextField *>(pSender);
                CC_BREAK_IF(!textField);
                std::string content = textField->getStringValue();
                if (content.length() == 0) {
                    content = "0";
                    textField->setText(content);
                }
                int percentage = atoi(content.c_str());
                setPercent(percentage);
                break;
            }
            default:
                break;
        }
    }
    
    SliderCheck::SliderCheck() :
    _sliderCheckBoxRenderer(nullptr),
    _checkBoxVisible(true)
    {
    }

    SliderCheck::~SliderCheck()
    {
        
    }
    
    SliderCheck * SliderCheck::create(Slider * slider, CheckBox * checkBox/* = nullptr*/)
    {
        SliderCheck * widget = new SliderCheck();
        if (slider && widget && widget->init())
        {
            widget->autorelease();
            widget->copyProperties(slider);
            widget->initWithSize();
            widget->setCheckBox(checkBox, true);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    SliderCheck * SliderCheck::create(const std::string& barName, const std::string& ballName, const std::string& progressName,
                                      const std::string& checkBg, const std::string& checkCross, TextureResType texType)
    {
        SliderCheck * widget = new SliderCheck();
        if (widget && widget->init())
        {
            widget->autorelease();
            widget->loadBarTexture(barName, texType);
            widget->loadSlidBallTextures(ballName, ballName, ballName, texType);
            widget->loadProgressBarTexture(progressName, texType);
            widget->initWithSize();
            widget->setCheckBox(checkBg, checkCross, texType);
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    void SliderCheck::initRenderer()
    {
        Slider::initRenderer();
        _sliderCheckBoxRenderer = CheckBox::create();
        static_cast<CheckBox *>(_sliderCheckBoxRenderer)->addEventListenerCheckBox(this, checkboxselectedeventselector(SliderCheck::onCheckBoxChanged));
        addProtectedChild(_sliderCheckBoxRenderer, -1, -1);
        
    }
    
    void SliderCheck::initWithSize()
    {
        Size size = getSize();
        float posX = _slidBallDisabledRenderer->getContentSize().width / 2 + 10;
        if (_checkBoxVisible) {
            posX += _sliderCheckBoxRenderer->getContentSize().width / 2;
        }
        setCheckBoxPosition(Point(-posX, size.height / 2), SIZE_ABSOLUTE);
    }
    
    void SliderCheck::setCheckBox(CheckBox * checkBox, bool defaultWhenNull/* = false*/)
    {
        removeChildByTag(SliderCheck::Tag::TAG_SLIDER_CHECKBOX);
        if (checkBox) {
            checkBox->setTag(SliderCheck::Tag::TAG_SLIDER_CHECKBOX);
            checkBox->setPosition(_sliderCheckBoxRenderer->getPosition());
            checkBox->setSelectedState(true);
            checkBox->addEventListenerCheckBox(this, checkboxselectedeventselector(SliderCheck::onCheckBoxChanged));
            _sliderCheckBoxRenderer->setVisible(false);
            if (checkBox->getParent()) {
                checkBox->removeFromParent();
            }
            addChild(checkBox);
        } else if (defaultWhenNull) {
            // do nothing
        }
    }
    
    void SliderCheck::setCheckBox(const std::string& bg, const std::string& cross, TextureResType texType/* = UI_TEX_TYPE_LOCAL*/)
    {
        CheckBox * checkBox = static_cast<CheckBox *>(_sliderCheckBoxRenderer);
        checkBox->loadTextures(bg, bg, cross, bg, cross, texType);
        checkBox->setSelectedState(true);
    }
    
    void SliderCheck::setCheckBoxPosition(const Point& pos, SizeType sizeType)
    {
        Point posAbsolute = Point(pos);
        if (sizeType == SIZE_PERCENT) {
            Size size = getSize();
            posAbsolute.x *= size.width;
            posAbsolute.y *= size.height;
        }
        _sliderCheckBoxRenderer->setPosition(posAbsolute);
        
        auto textField = getChildByTag(SliderCheck::Tag::TAG_SLIDER_CHECKBOX);
        if (textField) {
            textField->setPosition(posAbsolute);
        }
    }
    
    void SliderCheck::setCheckBoxVisible(bool visible)
    {
        if (_checkBoxVisible != visible) {
            _checkBoxVisible = visible;
            _sliderCheckBoxRenderer->setVisible(_checkBoxVisible);
            
            auto checkBox = getChildByTag(SliderCheck::Tag::TAG_SLIDER_CHECKBOX);
            if (checkBox) {
                checkBox->setVisible(_checkBoxVisible);
                _sliderCheckBoxRenderer->setVisible(false);
            }
        }
    }
    
    void SliderCheck::onCheckBoxChanged(Ref * pSender, CheckBoxEventType type)
    {
        if (pSender) {
            bool sliderEnable = (type == CHECKBOX_STATE_EVENT_SELECTED);
            setTouchEnabled(sliderEnable);
            setBright(sliderEnable);
        }
    }
    
        
    
} // ns ui

NS_CC_END
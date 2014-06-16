//
//  UIExtension.h
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#ifndef __CocosManual__UIExtension__
#define __CocosManual__UIExtension__

#include <iostream>
#include "ui/CocosGUI.h"

NS_CC_BEGIN

namespace ui {
    
#pragma mark - HelperExt
    class HelperExt : public Helper {
        
    };
    
#pragma mark - WidgetExt
    class WidgetExt : public Widget {
        
    };
    
#pragma mark - LayoutExt
    class LayoutExt : public Layout {
        
    };
    
#pragma mark - ButtonExt
    class ButtonExt : public Button {
        
    };
    
    class ButtonText : public Button {
        
    public:
        static ButtonText * create(const std::string& text, const Color3B& color,
                                   const std::string& fontName = "Helvetica", float size = 20);
        virtual bool init(const std::string& text, const Color3B& color, const std::string& fontName, float size);
        
    };

#pragma mark - CheckBoxExt
    class CheckBoxExt : public CheckBox {
        
    public:
        enum Tag {
            TAG_CHECKBOX_TITLE = 11,
        };
        
        CheckBoxExt();
        ~CheckBoxExt();
        
        static CheckBoxExt * create(CheckBox * checkBox, Label * label = nullptr);
        static CheckBoxExt * create(const std::string& bg, const std::string& cross, TextureResType texType,
                                    const std::string& fontName, const Color3B& fontColor, float fontSize,
                                    const std::string& title);
        
        void setTitle(Label * label, bool defaultWhenNull = false);
        void setTitle(const std::string& fontName, const Color3B& fontColor, float fontSize,
                      Color3B disableColor = Color3B::GRAY, bool shadow = false);
        void setTitleText(const std::string& title);
        void setTitleColor(Color3B color);
        void setTitleDisabledColor(Color3B color);
        void setTitlePosition(const Point& pos, SizeType sizeType);
        void setTitleVisible(bool visible);
        
    protected:
        virtual void initRenderer() override;
        virtual void onPressStateChangedToNormal() override;
        virtual void onPressStateChangedToDisabled() override;
        
        void initWithSize();
        void setTitleEnable(bool enable);
        
    protected:
        Node * _titleRenderer;
        bool _titleVisible;
        bool _titleEnable;
        Color3B _titleNormalColor;
        Color3B _titleDisabledColor;
    };
    
    class RadioGroup : public Widget {
    public:
        friend CheckBoxExt;
        
        RadioGroup();
        ~RadioGroup();
        
        static RadioGroup * create();
        static RadioGroup * create(CheckBox * checkBox, Vector<Label *> labels, const std::function<void(Ref *)> & callBack);
        static RadioGroup * create(const std::string& bg, const std::string& cross, TextureResType texType,
                                   const std::string& fontName, const Color3B& fontColor, float fontSize, int amount,
                                   const std::string title[], const std::function<void(Ref *)> & callBack);
        bool init();
        bool init(CheckBox * checkBox, Vector<Label *> labels, const std::function<void(Ref *)> & callBack);
        bool init(const std::string& bg, const std::string& cross, TextureResType texType,
                  const std::string& fontName, const Color3B& fontColor, float fontSize, int amount,
                  const std::string title[], const std::function<void(Ref *)> & callBack);
        void setItemPad(float pad = 5);
        void setSelectIndex(int index);
        void setSelectedChangedCallBack(const std::function<void(Ref *)> & callBack);
        
        int getSelectIndex();
        CheckBoxExt * getSelectItem();
        
    protected:
        void onCheckBoxChanged(Ref * pSender, CheckBoxEventType type);
        
    private:
        int _currItem;
        int _itemAmount;
        float _itemPad;
        std::function<void(Ref *)> _selectedChanged;
    };
    
#pragma mark - ImageViewExt
    class ImageViewExt : public ImageView {
        
    public:
        static ImageViewExt * create(const std::string& imageFileName, const Rect& rect, SizeType sizeType);
        virtual bool init(const std::string& imageFileName, const Rect& rect, SizeType sizeType);
        virtual void setTextureRect(const Rect& rect, SizeType sizeType);
    };

#pragma mark - TextExt
    class TextExt : public Text {
        
    };
    
    class TextAtlasExt : public TextAtlas {
        
    };
    
    class TextBMFontExt : public TextBMFont {
        
    };
    
    class RichTextExt : public RichText {
        
    };
    
    class TextFieldExt : public TextField {
        
    };
    
#pragma mark - LoadingBarExt
    class LoadingBarExt : public LoadingBar {
        
    public:
        enum Tag {
            TAG_LOADINGBAR_LABEL = 11,
        };
        
        LoadingBarExt();
        ~LoadingBarExt();
        
        static LoadingBarExt * create(LoadingBar * bar, Label * label = nullptr);
        static LoadingBarExt * create(const std::string& bg, const std::string& fg,
                                      TextureResType texType = UI_TEX_TYPE_LOCAL, int percentage = 0);
        static LoadingBarExt * create(const std::string& bg, const std::string& fg, TextureResType texType, int percentage,
                                      const std::string& fontName, const Color3B& fontColor, float fontSize);
        
        void loadBgTexture(const std::string& texture, TextureResType texType = UI_TEX_TYPE_LOCAL);
        void setLabelProgress(Label * label, bool defaultWhenNull = false);
        void setLabelProgress(const std::string& fontName, const Color3B& fontColor, float fontSize, bool shadow = true);
        void setLabelProgressPosition(const Point& pos, SizeType sizeType);
        void setLabelProgressVisible(bool visible);
        void setCallBack(const std::function<void(Ref*)> & callBack, bool onlyFinish = true);
        
        void setPercent(int percentage);
        
    protected:
        virtual void initRenderer() override;
        virtual void updateTextureColor() override;
        virtual void updateTextureOpacity() override;
        virtual void updateTextureRGBA() override;
        
        void initWithSize();
        
    protected:
        Node * _barBgRenderer;
        Node * _labPrgRenderer;
        TextureResType _renderBarBgTexType;
        std::string _textureBgFile;
        bool _prgVisible;
        bool _callBackOnlyFinish;
        std::function<void(Ref*)> _loadingCallBack;
    };
    
#pragma mark - ScrollViewExt
    class ScrollViewExt : public ScrollView {
        
    };
    
#pragma mark - ListViewExt
    class ListViewExt : public ListView {
        
    };
    
#pragma mark - PageViewExt
    class PageViewExt : public PageView {
        
    };
    
#pragma mark - SliderExt
    class SliderExt : public Slider {
        
    };
    
    class SliderFine : public Slider {
        
    public:
        enum Tag {
            TAG_SLIDER_TEXTFIELD = 11,
        };
        
        SliderFine();
        ~SliderFine();
        
        static SliderFine * create(Slider * slider, TextField * textField = nullptr);
        static SliderFine * create(const std::string& barName, const std::string& ballName,
                                   const std::string& progressName = "", TextureResType texType = UI_TEX_TYPE_LOCAL);
        static SliderFine * create(const std::string& barName, const std::string& ballName, const std::string& progressName,
                                   TextureResType texType, const std::string& fontName, Color3B fontColor, float fontSize);
        
        void setTextField(TextField * textField, bool defaultWhenNull = false);
        void setTextField(const std::string& fontName, Color3B fontColor, float fontSize);
        void setTextFieldPosition(const Point& pos, SizeType sizeType);
        void setTextFieldVisible(bool visible);
        void setTextFieldTouchEnable(bool enable);
        
        void setPercent(int percentage, bool manual = false);
        
    protected:
        virtual void initRenderer() override;
        virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
        virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
        
        void initWithSize();
        void onTextFieldChanged(Ref * pSender, TextFiledEventType type);
        
    protected:
        Node * _sliderTextFieldRenderer;
        bool _textFieldVisible;
        bool _textFieldEnable;
    };
    
    class SliderCheck : public Slider {
        
    public:
        enum Tag {
            TAG_SLIDER_CHECKBOX = 12,
        };
        
        SliderCheck();
        ~SliderCheck();
        
        static SliderCheck * create(Slider * slider, CheckBox * checkBox = nullptr);
        static SliderCheck * create(const std::string& barName, const std::string& ballName, const std::string& progressName,
                                    const std::string& checkBg, const std::string& checkCross, TextureResType texType);
        
        void setCheckBox(CheckBox * checkBox, bool defaultWhenNull = false);
        void setCheckBox(const std::string& bg, const std::string& cross, TextureResType texType = UI_TEX_TYPE_LOCAL);
        void setCheckBoxPosition(const Point& pos, SizeType sizeType);
        void setCheckBoxVisible(bool visible);
        
        
    protected:
        virtual void initRenderer() override;
        
        void initWithSize();
        void onCheckBoxChanged(Ref * pSender, CheckBoxEventType type);
        
    protected:
        Node * _sliderCheckBoxRenderer;
        bool _checkBoxVisible;
    };
    
#pragma mark - HBoxExt
    class HBoxExt : public HBox {
        
    public:
        void setLayoutAnchor(Point anchor);
    };
        
    
} // ns ui

NS_CC_END

#endif /* defined(__CocosManual__UIExtension__) */

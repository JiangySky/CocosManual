//
//  ManualDisplay.h
//  CocosManual
//
//  Created by Jiangy on 14-5-17.
//
//

#ifndef __CocosManual__ManualDisplay__
#define __CocosManual__ManualDisplay__

#include <iostream>
#include "GameInclude.h"

USING_NS_ALL

class ManualDisplay : public Layout {
    
    
public:
    enum Tag {
        TAG_LAYERCOLOR      = 300,
        TAG_LAYERGRADIENT   = 301,
        TAG_DRAWNODE        = 302,
        TAG_LAYERMULTIPLEX  = 303,
        TAG_NODESPRITE      = 304,
        TAG_SPRITEFRAME     = 305,
        TAG_MENUTITLE       = 306,
        TAG_SELECTSHOW      = 307,
        TAG_IMAGEVIEW       = 308,
        TAG_BUTTONTITLE     = 309,
        TAG_LOADINGLEFT     = 310,
        TAG_LOADINGRIGHT    = 311,
        TAG_LOADINGEXT      = 312,
        TAG_LOADINGEXTL     = 313,
        TAG_LOADINGEXTR     = 314,
        TAG_LOADINGEXTLL    = 315,
        TAG_EFFECT          = 316,
        TAG_SLIDERALL       = 317,
        TAG_SLIDERLEFT      = 318,
        TAG_SLIDERRIGHT     = 319,
        TAG_CHECKBOXTICK    = 320,
        TAG_CHECKBOXSTAR    = 321,
        TAG_ACTOR_MAN       = 322,
        TAG_ACTOR_WOLF      = 323,
        
    };
    
    enum ManualType {
        NONE,
        LAYERCOLOR,
        LAYERGRADIENT,
        LAYERMULTIPLEX,
        SPRITE,
        UIIMAGEVIEW,
        UILOADING,
        CONNECT,
    };
    
    CREATE_FUNC(ManualDisplay);
    virtual bool init();
    virtual void update(float time) override;
    
    ManualType manualType;
    void setManualType(ManualType type);
    
    void runCode(const char * key);
    
    void resetDisplay();
    
    void displayWelcome(const char * key);
    
    void displayScene();
    
    void displayLayer();
    void displayLayerColor();
    void displayLayerGradient();
    void displayLayerMultiplex();
    
    void displaySprite();
    void displayLabel();
    void displayMenu();
    
    void displayUILayout();
    void displayUIText();
    void displayUIImageView();
    void displayUIButton();
    void displayUILoadingBar();
    void displayUICheckBox();
    void displayUISlide();
    void displayUIListView();
    void displayUIScrollView();
    void displayUIPageView();
    void displayUIBox();
    
    void displayCCBuilder();
    void displayCCStudio();
    
    void displayTileMap();
    void displayParticle();
    
    void displayAction();
    
    void displayConnect();
    
    void onSwitchLayer(Ref * pSender, TouchEventType type);
    
    void addSpriteController();
    void onSetSpriteColor(Ref * pSender, TouchEventType type);
    void onSetSpriteFlip(Ref * pSender, TouchEventType type);
    void onSetSpriteRotate(Ref * pSender, TouchEventType type);
    void onSetSpriteVisible(Ref * pSender, TouchEventType type);
    void onSetSpriteSkew(Ref * pSender, TouchEventType type);
    void onSetSpriteScale(Ref * pSender, TouchEventType type);
    void onSetSpriteOpacity(Ref * pSender, TouchEventType type);
    void onSetSpriteBlendFunc(Ref * pSender, TouchEventType type);
    
    void addActionController();
    void onActionMove(Ref * pSender, TouchEventType type);
    void onActionRotate(Ref * pSender, TouchEventType type);
    void onActionVisible(Ref * pSender, TouchEventType type);
    void onActionSkew(Ref * pSender, TouchEventType type);
    void onActionScale(Ref * pSender, TouchEventType type);
    void onActionFade(Ref * pSender, TouchEventType type);
    void onActionTint(Ref * pSender, TouchEventType type);
    void onActionJump(Ref * pSender, TouchEventType type);
    void onActionBezier(Ref * pSender, TouchEventType type);
    void onActionTarget(Ref * pSender, TouchEventType type);
    
    void onTouchRichText(Ref * pSender, TouchEventType type);
    void onTouchButton(Ref * pSender, TouchEventType type);
    void onTouchCheckBox(Ref * pSender, CheckBoxEventType type);
    void onTouchSlider(Ref * pSender, SliderEventType type);
    void onTouchScollView(Ref * pSender, ScrollviewEventType type);
    void onTouchListView(Ref * pSender, ListViewEventType type);
    void onTouchPageView(Ref * pSender, PageViewEventType type);
    void onHttpRequstCallback(HttpClient * client, HttpResponse * response);
    void onHttpDownloadFailure(const char * errMsg, bool cancelDownload);
    
};

#endif /* defined(__CocosManual__ManualDisplay__) */

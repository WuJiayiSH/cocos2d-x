/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef cocos2d_libs_UIDeprecated_h
#define cocos2d_libs_UIDeprecated_h
/// @cond DO_NOT_SHOW

#include "platform/CCPlatformMacros.h"
#include "ui/UIWidget.h"
#include "ui/UILayout.h"
#include "ui/UIListView.h"
#include "ui/UILoadingBar.h"
#include "ui/UIPageView.h"
#include "ui/UIRichText.h"

NS_CC_BEGIN

namespace ui {
    
[[deprecated]] const Widget::PositionType POSITION_ABSOLUTE = Widget::PositionType::ABSOLUTE;
[[deprecated]] const Widget::PositionType POSITION_PERCENT = Widget::PositionType::PERCENT;
[[deprecated]] const Widget::SizeType SIZE_ABSOLUTE = Widget::SizeType::ABSOLUTE;
[[deprecated]] const Widget::SizeType SIZE_PERCENT = Widget::SizeType::PERCENT;
[[deprecated]] const Widget::TextureResType UI_TEX_TYPE_LOCAL = Widget::TextureResType::LOCAL;
[[deprecated]] const Widget::TextureResType UI_TEX_TYPE_PLIST = Widget::TextureResType::PLIST;
[[deprecated]] const Widget::BrightStyle BRIGHT_NONE = Widget::BrightStyle::NONE;
[[deprecated]] const Widget::BrightStyle BRIGHT_NORMAL = Widget::BrightStyle::NORMAL;
[[deprecated]] const Widget::BrightStyle BRIGHT_HIGHLIGHT = Widget::BrightStyle::HIGHLIGHT;
[[deprecated]] const Layout::ClippingType LAYOUT_CLIPPING_STENCIL = Layout::ClippingType::STENCIL;
[[deprecated]] const Layout::ClippingType LAYOUT_CLIPPING_SCISSOR = Layout::ClippingType::SCISSOR;
[[deprecated]] const Layout::Type LAYOUT_ABSOLUTE = Layout::Type::ABSOLUTE;
[[deprecated]] const Layout::Type LAYOUT_LINEAR_VERTICAL = Layout::Type::VERTICAL;
[[deprecated]] const Layout::Type LAYOUT_LINEAR_HORIZONTAL = Layout::Type::HORIZONTAL;
[[deprecated]] const Layout::Type LAYOUT_RELATIVE = Layout::Type::VERTICAL;
[[deprecated]] const Layout::BackGroundColorType LAYOUT_COLOR_NONE = Layout::BackGroundColorType::NONE;
[[deprecated]] const Layout::BackGroundColorType LAYOUT_COLOR_SOLID = Layout::BackGroundColorType::SOLID;
[[deprecated]] const Layout::BackGroundColorType LAYOUT_COLOR_GRADIENT
                                                        = Layout::BackGroundColorType::GRADIENT;
[[deprecated]] const LayoutParameter::Type LAYOUT_PARAMETER_NONE = LayoutParameter::Type::NONE;
[[deprecated]] const LayoutParameter::Type LAYOUT_PARAMETER_LINEAR = LayoutParameter::Type::LINEAR;
[[deprecated]] const LayoutParameter::Type LAYOUT_PARAMETER_RELATIVE = LayoutParameter::Type::RELATIVE;
[[deprecated]] const LinearLayoutParameter::LinearGravity LINEAR_GRAVITY_NONE
    = LinearLayoutParameter::LinearGravity::NONE;
[[deprecated]] const LinearLayoutParameter::LinearGravity LINEAR_GRAVITY_LEFT
    = LinearLayoutParameter::LinearGravity::LEFT;
[[deprecated]] const LinearLayoutParameter::LinearGravity LINEAR_GRAVITY_TOP
    = LinearLayoutParameter::LinearGravity::TOP;
[[deprecated]] const LinearLayoutParameter::LinearGravity LINEAR_GRAVITY_RIGHT
    = LinearLayoutParameter::LinearGravity::RIGHT;
[[deprecated]] const LinearLayoutParameter::LinearGravity LINEAR_GRAVITY_BOTTOM
    = LinearLayoutParameter::LinearGravity::BOTTOM;
[[deprecated]] const LinearLayoutParameter::LinearGravity LINEAR_GRAVITY_CENTER_VERTICAL
    = LinearLayoutParameter::LinearGravity::CENTER_VERTICAL;
[[deprecated]] const LinearLayoutParameter::LinearGravity LINEAR_GRAVITY_CENTER_HORIZONTAL
    = LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL;
    
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_NONE
    = RelativeLayoutParameter::RelativeAlign::NONE;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_TOP_LEFT
    = RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_TOP_CENTER_HORIZONTAL
    = RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_TOP_RIGHT
    = RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_LEFT_CENTER_VERTICAL
    = RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_CENTER_IN_PARENT
    = RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_RIGHT_CENTER_VERTICAL
    = RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_LEFT_BOTTOM
    = RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_BOTTOM_CENTER_HORIZONTAL
    = RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_ALIGN_PARENT_RIGHT_BOTTOM
    = RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_ABOVE_LEFTALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_ABOVE_LEFTALIGN;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_ABOVE_CENTER
    = RelativeLayoutParameter::RelativeAlign::LOCATION_ABOVE_CENTER;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_ABOVE_RIGHTALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_ABOVE_RIGHTALIGN;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_LEFT_OF_TOPALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_LEFT_OF_TOPALIGN;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_LEFT_OF_CENTER
    = RelativeLayoutParameter::RelativeAlign::LOCATION_LEFT_OF_CENTER;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_LEFT_OF_BOTTOMALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_LEFT_OF_BOTTOMALIGN;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_RIGHT_OF_TOPALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_RIGHT_OF_TOPALIGN;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_RIGHT_OF_CENTER
    = RelativeLayoutParameter::RelativeAlign::LOCATION_RIGHT_OF_CENTER;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_RIGHT_OF_BOTTOMALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_RIGHT_OF_BOTTOMALIGN;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_BELOW_LEFTALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_LEFTALIGN;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_BELOW_CENTER
    = RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_CENTER;
[[deprecated]] const RelativeLayoutParameter::RelativeAlign RELATIVE_LOCATION_BELOW_RIGHTALIGN
    = RelativeLayoutParameter::RelativeAlign::LOCATION_BELOW_RIGHTALIGN;

[[deprecated]] const ListView::Gravity LISTVIEW_GRAVITY_LEFT = ListView::Gravity::LEFT;
[[deprecated]] const ListView::Gravity LISTVIEW_GRAVITY_RIGHT = ListView::Gravity::RIGHT;
[[deprecated]] const ListView::Gravity LISTVIEW_GRAVITY_CENTER_HORIZONTAL = ListView::Gravity::CENTER_HORIZONTAL;
[[deprecated]] const ListView::Gravity LISTVIEW_GRAVITY_TOP = ListView::Gravity::TOP;
[[deprecated]] const ListView::Gravity LISTVIEW_GRAVITY_BOTTOM = ListView::Gravity::BOTTOM;
[[deprecated]] const ListView::Gravity LISTVIEW_GRAVITY_CENTER_VERTICAL = ListView::Gravity::CENTER_VERTICAL;
[[deprecated]] const LoadingBar::Direction LoadingBarTypeLeft =  LoadingBar::Direction::LEFT;
[[deprecated]] const LoadingBar::Direction LoadingBarTypeRight =  LoadingBar::Direction::RIGHT;
[[deprecated]] const PageView::TouchDirection PAGEVIEW_TOUCHLEFT =  PageView::TouchDirection::LEFT;
[[deprecated]] const PageView::TouchDirection PAGEVIEW_TOUCHRIGHT =  PageView::TouchDirection::RIGHT;
[[deprecated]] const RichElement::Type RICH_TEXT =  RichElement::Type::TEXT;
[[deprecated]] const RichElement::Type RICH_IMAGE =  RichElement::Type::IMAGE;
[[deprecated]] const RichElement::Type RICH_CUSTOM =  RichElement::Type::CUSTOM;
[[deprecated]] const ScrollView::Direction SCROLLVIEW_DIR_NONE =  ScrollView::Direction::NONE;
[[deprecated]] const ScrollView::Direction SCROLLVIEW_DIR_VERTICAL =  ScrollView::Direction::VERTICAL;
[[deprecated]] const ScrollView::Direction SCROLLVIEW_DIR_HORIZONTAL =  ScrollView::Direction::HORIZONTAL;
[[deprecated]] const ScrollView::Direction SCROLLVIEW_DIR_BOTH =  ScrollView::Direction::BOTH;

    
[[deprecated]] typedef Widget::TextureResType TextureResType;
[[deprecated]] typedef Widget::PositionType   PositionType;
[[deprecated]] typedef Widget::SizeType SizeType;
[[deprecated]] typedef Widget::BrightStyle BrightStyle;
[[deprecated]] typedef Layout::ClippingType LayoutClippingType;
[[deprecated]] typedef Layout::Type LayoutType;
[[deprecated]] typedef Layout::BackGroundColorType LayoutBackGroundColorType;
[[deprecated]] typedef LayoutParameter::Type LayoutParameterType;
[[deprecated]] typedef LinearLayoutParameter::LinearGravity LinearGravity;
[[deprecated]] typedef RelativeLayoutParameter::RelativeAlign RelativeAlign;
[[deprecated]] typedef ListView::Gravity ListViewGravity;
[[deprecated]] typedef LoadingBar::Direction LoadingBarType;
[[deprecated]] typedef PageView::TouchDirection PVTouchDir;
[[deprecated]] typedef RichElement::Type RichElementType;
[[deprecated]] typedef ScrollView::Direction SCROLLVIEW_DIR;
[[deprecated]] extern const Margin MarginZero;


}


NS_CC_END

/// @endcond
#endif

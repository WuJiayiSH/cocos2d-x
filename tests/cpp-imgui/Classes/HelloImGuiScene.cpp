/****************************************************************************
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

#include "HelloImGuiScene.h"
#include "CCImGuiWidgets.h"
#include "imgui.h"

USING_NS_CC;

namespace
{
    bool openDemo = false;
    bool openInspector = true;
}

Scene* HelloImGuiScene::scene()
{
    return HelloImGuiScene::create();
}

bool HelloImGuiScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    Label* intro = Label::createWithSystemFont("Inspector window shows a tree contains\nall nodes in the current scene.\n\nBy selecting single node, you can\nmodify properties of that node at runtime.\n\nYou can also Click [Toggle Demo]\nto play with the ImGui demo.", "Arial", 22);
    intro->setPosition(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.74);
    this->addChild(intro);

    Label* labelInspctor = Label::createWithSystemFont("[ Toggle Inspector ]", "Arial", 28);
    labelInspctor->enableUnderline();
    MenuItemLabel* menuItemInspctor = MenuItemLabel::create(labelInspctor, CC_CALLBACK_1(HelloImGuiScene::onToggleInspector,this));
    menuItemInspctor->setPosition(origin.x + visibleSize.width * 0.73, origin.y + visibleSize.height / 2 - 100);

    Label* labelDemo = Label::createWithSystemFont("[ Toggle Demo ]", "Arial", 28);
    labelDemo->enableUnderline();
    MenuItemLabel* menuItemDemo = MenuItemLabel::create(labelDemo, CC_CALLBACK_1(HelloImGuiScene::onToggleDemo,this));
    menuItemDemo->setPosition(origin.x + visibleSize.width * 0.73, origin.y + visibleSize.height / 2 - 200);

    auto menu = Menu::create(menuItemInspctor, menuItemDemo, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

	Director::getInstance()->getImGuiManager()->addCallback([]() {
        if (openDemo)
		    ImGui::ShowDemoWindow(&openDemo);

        if (openInspector)
            CCImGuiWidgets::ShowInspector(&openInspector);
	}, "hello");
    return true;
}

void HelloImGuiScene::onToggleInspector(Ref*)
{
    openInspector = !openInspector;
}

void HelloImGuiScene::onToggleDemo(Ref*)
{
    openDemo = !openDemo;
}

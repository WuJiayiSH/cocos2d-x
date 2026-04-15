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

#include "cocos2d.h"
#include "AppDelegate.h"
#include "audio/include/SimpleAudioEngine.h"
#include "base/CCScriptSupport.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"

#include "Editor.h"
#include "WidgetFactory.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    // end simple audio engine here, or it may crashed on win32
    SimpleAudioEngine::getInstance()->end();
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    GLView* glview = Director::getInstance()->getOpenGLView();
    if (!glview)
    {
        glview = GLViewImpl::createWithMaximized("Cocos2d-x - Lua", 1.0f, true);
        Director::getInstance()->setOpenGLView(glview);
    }

    // set low FPS to reduce CPU/GPU usage for editor
    Director::getInstance()->setAnimationInterval(1.0f / 25.0f);

    // register lua engine
    LuaEngine* engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);

    FileUtils::getInstance()->addSearchPath("src");
    FileUtils::getInstance()->addSearchPath("res");
    
    // load configuration file
    Configuration::getInstance()->loadConfigFile("config.plist");

    const char* str = R"(
local fileUtils = cc.FileUtils:getInstance()
fileUtils:setPopupNotify(false)

DEBUG = 2
CC_USE_FRAMEWORK = true
CC_DISABLE_GLOBAL = true

require "cocos.cocos2d.json"
require "cocos.init"
)";

    if (engine->executeString(str))
    {
        return false;
    }

    // register editor related nodes, components and widgets
    CCImEditor::Editor* editor = CCImEditor::Editor::getInstance();
    if (!editor)
        return false;

    editor->registerNodes();
    editor->registerComponents();
    editor->registerWidgets();

    // register your own node, component and widget here

    // you can also add your custom runnable
    // editor->addRunnable("Run My Game", []() {
    //     Director::getInstance()->runWithScene(MyGameScene::create());
    // });

    std::string widgetNames[] = {
        "CCImEditor.NodeProperties",
        "CCImEditor.NodeTree",
        "CCImEditor.Viewport",
        "CCImEditor.Assets",
        "CCImEditor.Console",
        "CCImEditor.Animation",
    };

    for (const auto& widgetName : widgetNames)
    {
        if (CCImEditor::Widget* widget = CCImEditor::WidgetFactory::getInstance()->createWidget(widgetName.c_str()))
        {
            editor->addWidget(widget);
        }
    }

    Scene* scene = Scene::create();
    scene->addChild(editor);
    Director::getInstance()->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}


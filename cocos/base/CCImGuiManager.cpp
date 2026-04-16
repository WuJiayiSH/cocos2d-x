#include "CCImGuiManager.h"

#include "imgui_impl_cocos2dx.h"
#include "CCDirector.h"
#include "CCEventDispatcher.h"

NS_CC_BEGIN

void ImGuiManager::beginFrame()
{
#if CC_USE_IMGUI
    if (!_initialized)
        return;

    // create frame
    ImGui_ImplCocos2dx_NewFrame();

    // draw all gui
    for (const auto& callback : _callbacks)
    {
        callback.second();
    }
#endif
}

void ImGuiManager::endFrame()
{
#if CC_USE_IMGUI
    if (!_initialized)
        return;

    // render
    ImGui::Render();
    ImGui_ImplCocos2dx_RenderDrawData(ImGui::GetDrawData());
#endif
}

void ImGuiManager::addCallback(const std::function<void()>& callBack, const std::string& name)
{
	_callbacks[name] = callBack;
}

void ImGuiManager::removeCallback(const std::string& name)
{
	const auto iter = _callbacks.find(name);
	if (iter != _callbacks.end())
		_callbacks.erase(iter);
}

void ImGuiManager::init()
{
#if CC_USE_IMGUI
    if (_initialized)
        return;

    ImGui_ImplCocos2dx_Init(true);

    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = [](Touch* touch, Event*) -> bool {
        return ImGui::IsAnyItemHovered();
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener, -INT_MAX);

    _initialized = true;
#endif
}

void ImGuiManager::shutdown()
{
#if CC_USE_IMGUI
    if (!_initialized)
        return;

    Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);

    ImGui_ImplCocos2dx_Shutdown();

    _initialized = false;
#endif
}


NS_CC_END
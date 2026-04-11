#ifndef __CC_IMGUI_MANAGER_H__
#define __CC_IMGUI_MANAGER_H__

#include <string>
#include <unordered_map>
#include <functional>

#include "base/CCEventListenerTouch.h"
#include "base/CCRefPtr.h"

NS_CC_BEGIN

class ImGuiManager
{
    friend class Director;

public:
    /**
     * Add a callback to the ImGuiManager, the callback will be called in every frame, and you can draw your GUI in the callback.
     */
    void addCallback(const std::function<void()>& callBack, const std::string& name);

    /**
     * Remove the callback with the given name.
     */
	void removeCallback(const std::string& name);

private:
    void beginFrame();
    void endFrame();

    void init();
    void shutdown();

    std::unordered_map<std::string, std::function<void()>> _callbacks;
    RefPtr<EventListenerTouchOneByOne> _touchListener;
    bool _initialized = false;
};

NS_CC_END
#endif // __CC_IMGUI_MANAGER_H__
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

#ifndef __CC_FRAMEWORK_COMCONTAINER_H__
#define __CC_FRAMEWORK_COMCONTAINER_H__

/// @cond DO_NOT_SHOW

#include "base/CCMap.h"
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <vector>

NS_CC_BEGIN

class Component;
class Node;

class CC_DLL ComponentContainer
{
protected:
    /**
     * @js ctor
     */
    ComponentContainer(Node* node);
    
public:
    /**
     * @js NA
     * @lua NA
     */
    ~ComponentContainer();
    
	/**
     * @js getComponent
     */
	Component* get(const std::string& name) const;

    /**
     * Get component by type. Returns the first component of the specified type.
     * @param T The component type to search for
     * @return Pointer to component of type T, or nullptr if not found
     */
    template<typename T>
    T* get() const
    {
        std::type_index typeIdx(typeid(T));
        auto it = _componentsByType.find(typeIdx);
        if (it != _componentsByType.end() && !it->second.empty())
        {
            return static_cast<T*>(it->second[0]);
        }
        return nullptr;
    }

    /**
     * Get all components of a specific type.
     * @param T The component type to search for
     * @return Vector of pointers to components of type T
     */
    template<typename T>
    std::vector<T*> getAll() const
    {
        std::vector<T*> results;
        std::type_index typeIdx(typeid(T));
        auto it = _componentsByType.find(typeIdx);
        if (it != _componentsByType.end())
        {
            for (Component* comp : it->second)
            {
                results.push_back(static_cast<T*>(comp));
            }
        }
        return results;
    }

    bool add(Component *com);
    bool remove(const std::string& name);
    bool remove(Component *com);
    void removeAll();
    void visit(float delta);
    
    void onEnter();
    void onExit();
    
    bool isEmpty() const { return _componentsByName.empty(); } 
private:
    std::unordered_map<std::string, Component*> _componentsByName;
    std::unordered_map<std::type_index, std::vector<Component*>> _componentsByType;
    Node *_owner;
    
    friend class Node;
};

NS_CC_END

/// @endcond
#endif  // __CC_FRAMEWORK_COMCONTAINER_H__

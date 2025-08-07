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


#include "2d/CCComponentContainer.h"
#include "2d/CCComponent.h"
#include "2d/CCNode.h"
#include <algorithm>

NS_CC_BEGIN

ComponentContainer::ComponentContainer(Node* node)
: _owner(node)
{
}

ComponentContainer::~ComponentContainer()
{  
}

Component* ComponentContainer::get(const std::string& name) const
{
    auto it = _componentsByName.find(name);
    if (it != _componentsByName.end())
    {
        return it->second;
    }
    return nullptr;
}

bool ComponentContainer::add(Component *com)
{
    bool ret = false;
    CCASSERT(com != nullptr, "Component must be non-nil");
    CCASSERT(com->getOwner() == nullptr, "Component already added. It can't be added again");
    do
    {
        auto componentName = com->getName();

        if (_componentsByName.find(componentName) != _componentsByName.end())
        {
            CCASSERT(false, "ComponentContainer already have this kind of component");
            break;
        }
        
        // Add to name-based map
        _componentsByName[componentName] = com;
        
        // Add to type-based map
        std::type_index typeIdx(typeid(*com));
        _componentsByType[typeIdx].push_back(com);
        
        com->retain();
        com->setOwner(_owner);
        com->onAdd();

        ret = true;
    } while(0);
    return ret;
}

bool ComponentContainer::remove(const std::string& componentName)
{
    bool ret = false;
    do 
    {        
        auto iter = _componentsByName.find(componentName);
        if (iter == _componentsByName.end())
            break;

        auto component = iter->second;
        
        // Remove from name-based map
        _componentsByName.erase(componentName);
        
        // Remove from type-based map
        std::type_index typeIdx(typeid(*component));
        auto typeIter = _componentsByType.find(typeIdx);
        if (typeIter != _componentsByType.end())
        {
            auto& typeVector = typeIter->second;
            typeVector.erase(std::remove(typeVector.begin(), typeVector.end(), component), typeVector.end());
            
            // Remove the type entry if no components of this type remain
            if (typeVector.empty())
            {
                _componentsByType.erase(typeIter);
            }
        }

        component->onRemove();
        component->setOwner(nullptr);
        component->release();

        ret = true;
    } while(0);

    return ret;
 }

bool ComponentContainer::remove(Component *com)
{
    return remove(com->getName());
}

void ComponentContainer::removeAll()
{
    if (!_componentsByName.empty())
    {
        for (auto& iter : _componentsByName)
        {
            iter.second->onRemove();
            iter.second->setOwner(nullptr);
            iter.second->release();
        }
        
        _componentsByName.clear();
        _componentsByType.clear();
        _owner->unscheduleUpdate();
    }
}

void ComponentContainer::visit(float delta)
{
    if (!_componentsByName.empty())
    {
        CC_SAFE_RETAIN(_owner);
        for (auto& iter : _componentsByName)
        {
            iter.second->update(delta);
        }
        CC_SAFE_RELEASE(_owner);
    }
}

void ComponentContainer::onEnter()
{
    for (auto& iter : _componentsByName)
    {
        iter.second->onEnter();
    }
}

void ComponentContainer::onExit()
{
    for (auto& iter : _componentsByName)
    {
        iter.second->onExit();
    }
}

NS_CC_END

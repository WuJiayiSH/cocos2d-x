/*
 
 Copyright (c) 2012 - Zynga Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
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
 
 */

/*
 Local Storage support for the JS Bindings for iOS.
 Works on cocos2d-iphone and cocos2d-x.
 */

#include "storage/local-storage/LocalStorage.h"
#include "platform/CCPlatformMacros.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_EMSCRIPTEN)

namespace {
    std::string s_localStorageFullpath;
    cocos2d::ValueMap s_localStorageCache;
    bool s_localStorageInitialized = false;
}

void localStorageInit( const std::string& fullpath)
{
    if (!s_localStorageInitialized)
    {
        s_localStorageInitialized = true;

        s_localStorageFullpath = fullpath;

        // If the fullpath is empty, it will create an in-memory cache.
        if (s_localStorageFullpath.empty())
            s_localStorageCache.clear();
        else
            s_localStorageCache = cocos2d::FileUtils::getInstance()->getValueMapFromFile(fullpath);
    }
}

void localStorageFree()
{
    if (s_localStorageInitialized)
    {
        s_localStorageInitialized = false;
        s_localStorageCache.clear();
    }
}

/** sets an item in the LS */
void localStorageSetItem( const std::string& key, const std::string& value)
{
    if (!s_localStorageInitialized)
    {
        CCLOGERROR("localStorageSetItem() called before localStorageInit().");
        return;
    }

    s_localStorageCache[key] = value;

    if (!s_localStorageFullpath.empty())
        cocos2d::FileUtils::getInstance()->writeToFile(s_localStorageCache, s_localStorageFullpath);
}

/** gets an item from the LS */
bool localStorageGetItem( const std::string& key, std::string *outItem )
{
    if (!s_localStorageInitialized)
    {
        CCLOGERROR("localStorageGetItem() called before localStorageInit().");
        return false;
    }

    if (outItem == nullptr)
        return false;

    auto iter = s_localStorageCache.find(key);
    if (iter != s_localStorageCache.end())
    {
        *outItem = iter->second.asString();
        return true;
    }
    return false;
}

/** removes an item from the LS */
void localStorageRemoveItem( const std::string& key )
{
    if (!s_localStorageInitialized)
    {
        CCLOGERROR("localStorageRemoveItem() called before localStorageInit().");
        return;
    }

    s_localStorageCache.erase(key);
    if (!s_localStorageFullpath.empty())
        cocos2d::FileUtils::getInstance()->writeToFile(s_localStorageCache, s_localStorageFullpath);
}

/** removes all items from the LS */
void localStorageClear()
{
    if (!s_localStorageInitialized)
    {
        CCLOGERROR("localStorageClear() called before localStorageInit().");
        return;
    }

    s_localStorageCache.clear();
    if (!s_localStorageFullpath.empty())
        cocos2d::FileUtils::getInstance()->writeToFile(s_localStorageCache, s_localStorageFullpath);
}

#endif // #if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_EMSCRIPTEN)

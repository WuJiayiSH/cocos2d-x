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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)

#include <emscripten.h>

void localStorageInit( const std::string& fullpath/* = "" */)
{
    // for jsb only
}

void localStorageFree()
{
    // for jsb only
}

/** sets an item in the LS */
void localStorageSetItem( const std::string& key, const std::string& value)
{
    EM_ASM_ARGS({
        localStorage.setItem(UTF8ToString($0), UTF8ToString($1));
    }, key.c_str(), value.c_str());
}

/** gets an item from the LS */
bool localStorageGetItem( const std::string& key, std::string *outItem )
{
    char *str = (char*)EM_ASM_INT({
        var jsString = localStorage.getItem(UTF8ToString($0));
        if (!jsString)
            return 0;

        var lengthBytes = lengthBytesUTF8(jsString)+1;
        // 'jsString.length' would return the length of the string as UTF-16
        // units, but Emscripten C strings operate as UTF-8.
        var stringOnWasmHeap = _malloc(lengthBytes);
        stringToUTF8(jsString, stringOnWasmHeap, lengthBytes);
        return stringOnWasmHeap;
    }, key.c_str());

    if (str)
    {
        if (outItem)
            outItem->assign(str, strlen(str));

        free(str);
        return true;
    }
    else
    {
        return false;
    }
}

/** removes an item from the LS */
void localStorageRemoveItem( const std::string& key )
{
    EM_ASM_ARGS({
        localStorage.removeItem(UTF8ToString($0));
    }, key.c_str());
}

/** removes all items from the LS */
void localStorageClear()
{
    EM_ASM(
        localStorage.clear();
    );
}

#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)

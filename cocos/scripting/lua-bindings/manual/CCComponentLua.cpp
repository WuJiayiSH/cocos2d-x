/****************************************************************************
 Copyright (c) 2015-2016 Chukong Technologies Inc.
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

#include "scripting/lua-bindings/manual/CCComponentLua.h"
#include <string>
#include "base/CCScriptSupport.h"
#include "platform/CCFileUtils.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

NS_CC_BEGIN

namespace {
    const std::string ON_ENTER = "onEnter";
    const std::string ON_EXIT = "onExit";
    const std::string UPDATE = "update";
    
    const char* const KEY_COMPONENT = "component";

    // the index used to get lua table, it is unique for every component
    int s_index = 0;
}


namespace {
    void adjustScriptFileName(std::string& scriptFileName)
    {
        assert(scriptFileName.size() > 4);
        
        auto fileUtils = FileUtils::getInstance();
        if (fileUtils->isFileExist(scriptFileName))
            return;
        
        const std::string luaSuffix(".lua");
        const std::string luacSuffix(".luac");
        
        // xxx.lua -> xxx.luac or
        // xxx.luac -> xxx.lua
        if (scriptFileName.compare(scriptFileName.size() - luaSuffix.size(), luaSuffix.size(), luaSuffix) == 0)
            scriptFileName.replace(scriptFileName.size() - luaSuffix.size(), luaSuffix.size(), luacSuffix);
        else
            scriptFileName.replace(scriptFileName.size() - luacSuffix.size(), luacSuffix.size(), luaSuffix);
    }
}

ComponentLua* ComponentLua::create()
{
    initClass();
    auto componentLua = new(std::nothrow) ComponentLua();
    if (componentLua)
    {
        componentLua->autorelease();
    }
    
    return componentLua;
}

ComponentLua* ComponentLua::create(std::string_view scriptFileName)
{
    CC_ASSERT(!scriptFileName.empty());
    
    initClass();

    std::string outFilename(scriptFileName);
    adjustScriptFileName(outFilename);
    auto componentLua = new(std::nothrow) ComponentLua(outFilename);
    if (componentLua)
    {
        componentLua->autorelease();
    }
    
    return componentLua;
}

ComponentLua::ComponentLua()
: _table(nullptr)
, _succeedLoadingScript(false)
{
}

ComponentLua::ComponentLua(std::string_view scriptFileName)
: _scriptFileName(scriptFileName)
, _table(nullptr)
{
    _succeedLoadingScript = loadAndExecuteScript();
}

ComponentLua::~ComponentLua()
{
    removeLuaTable();
}

bool ComponentLua::loadAndExecuteScript(std::string_view scriptFileName)
{
    removeLuaTable();

    _scriptFileName = scriptFileName;
    _succeedLoadingScript = loadAndExecuteScript();
    return _succeedLoadingScript;
}

void ComponentLua::getScriptObjectInternal() const
{
    lua_State *l = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    lua_pushstring(l, KEY_COMPONENT);      // stack: "component"
    lua_rawget(l, LUA_REGISTRYINDEX);      // stack: LUA_REGISTRYINDEX["component"]
    lua_pushstring(l, _strIndex.c_str());  // stack: LUA_REGISTRYINDEX["component"] strIndex
    lua_rawget(l, -2);                     // stack: LUA_REGISTRYINDEX["component"]
}

void* ComponentLua::getScriptObject() const
{
    getScriptObjectInternal();
    return nullptr;
}

void ComponentLua::update(float delta)
{
    if (_succeedLoadingScript && getLuaFunction(UPDATE))
    {
        getUserData();
        lua_State *l = LuaEngine::getInstance()->getLuaStack()->getLuaState();
        lua_pushnumber(l, delta);
        LuaEngine::getInstance()->getLuaStack()->executeFunction(2);
    }
}

void ComponentLua::onEnter()
{
    if (_succeedLoadingScript && getLuaFunction(ON_ENTER))
    {
        getUserData();
        LuaEngine::getInstance()->getLuaStack()->executeFunction(1);
    }
}

void ComponentLua::onExit()
{
    if (_succeedLoadingScript && getLuaFunction(ON_EXIT))
    {
        getUserData();
        LuaEngine::getInstance()->getLuaStack()->executeFunction(1);
    }
}

bool ComponentLua::getLuaFunction(const std::string& functionName)
{
    lua_State *l = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    
    lua_pushstring(l, KEY_COMPONENT);                // stack: "component"
    lua_rawget(l, LUA_REGISTRYINDEX);                // stack: table_of_component
    lua_pushstring(l, _strIndex.c_str());            // stack: table_of_component strIndex
    lua_rawget(l, -2);                               // stack: table_of_component table_of_this
    lua_pushstring(l, functionName.c_str());         // stack: table_of_component table_of_this "update"
    // lua_gettable will trigger a metamethod for the "index" event which allows inheritance of Lua Components
    lua_gettable(l, -2);                             // stack: table_of_component table_of_this table_of_this["update"]
    lua_remove(l, -2);                               // stack: table_of_component table_of_this["update"]
    lua_remove(l, -2);                               // stack: table_of_this["update"]
    
    int type = lua_type(l, -1);
    if (type != LUA_TFUNCTION)
    {
        lua_pop(l, 1);
    }
    
    return type == LUA_TFUNCTION;
}

bool ComponentLua::loadAndExecuteScript()
{
    // register native functions
    auto engine = LuaEngine::getInstance();
    lua_State *l = engine->getLuaStack()->getLuaState();
    
    // load script
    auto fileUtils = FileUtils::getInstance();
    std::string fullPathOfScript = fileUtils->fullPathForFilename(_scriptFileName);
    Data data = fileUtils->getDataFromFile(fullPathOfScript);
    int error = LUA_ERRFILE;
    if(data.getSize() > 0)
        error = engine->getLuaStack()->luaLoadBuffer(l, (const char*)data.getBytes(), (int)data.getSize(), fullPathOfScript.c_str());
    if (error)
    {
        CCLOG("ComponentLua::loadAndExecuteScript: %s", lua_tostring(l, -1));
        lua_pop(l, 1);
        return false;
    }
    
    // execute script
    error = lua_pcall(l, 0, 1, 0);
    if (error)
    {
        CCLOG("ComponentLua::loadAndExecuteScript: %s", lua_tostring(l, -1));
        lua_pop(l, 1);
        return false;
    }
    
    // check the return value from lua script is a table
    int type = lua_type(l, -1);
    if (type != LUA_TTABLE)
    {
        CCLOG("%s should return a table, or the script component can not work currectly", _scriptFileName.c_str());
        lua_pop(l, 1);
        return false;
    }
    
    storeLuaTable();
    return true;
}

void ComponentLua::initClass()
{
    static bool run = true;
    if (run)
    {
        // LUA_REGISTRYINDEX["component"] = new table
        
        LuaEngine* engine = LuaEngine::getInstance();
        lua_State* l = engine->getLuaStack()->getLuaState();
        
        lua_pushstring(l, KEY_COMPONENT);              // stack: "component"
        lua_newtable(l);                               // stack: "component" table
        lua_rawset(l, LUA_REGISTRYINDEX);              // stack: -
        
        run = false;
    }
    
    
}

void ComponentLua::storeLuaTable()
{
    lua_State *l = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    
    s_index++;
    _strIndex.append(StringUtils::toString(s_index));
    
    // LUA_REGISTRYINDEX["component"][strIndex] = table return from lua
    lua_pushstring(l, KEY_COMPONENT);          // stack: table_return_from_lua "component"
    lua_rawget(l, LUA_REGISTRYINDEX);          // stack: table_return_from_lua table_of_component
    lua_pushstring(l, _strIndex.c_str());      // stack: table_return_from_lua table_of_component strIndex
    lua_pushvalue(l, -3);                      // stack: table_return_from_lua table_of_component strIndex table_return_from_lua
    lua_rawset(l, -3);                         // stack: table_return_from_lua table_of_component
    lua_pop(l, 1);                             // stack: table_return_from_lua
    
    // local peer = {}
    // table_return_from_lua.__index = table_return_from_lua.__index or table_return_from_lua
    // setmetatable(peer, table_return_from_lua)
    // tolua.setpeer(userdata, peer)
    object_to_luaval<cocos2d::ComponentLua>(l, "cc.ComponentLua", this);  // stack: table_return_from_lua userdata
    lua_newtable(l);                            // stack: table_return_from_lua userdata peer
    lua_pushvalue(l, -3);                       // stack: table_return_from_lua userdata peer table_return_from_lua
    lua_pushstring(l, "__index");               // stack: table_return_from_lua userdata peer table_return_from_lua "__index"
    lua_rawget(l, -2);                          // stack: table_return_from_lua userdata peer table_return_from_lua __index

    if (lua_isnil(l, -1))
    {
        lua_pop(l, 1);                          // stack: table_return_from_lua userdata peer table_return_from_lua
        lua_pushstring(l, "__index");           // stack: table_return_from_lua userdata peer table_return_from_lua "__index"
        lua_pushvalue(l, -2);                   // stack: table_return_from_lua userdata peer table_return_from_lua "__index" table_return_from_lua
        lua_rawset(l, -3);                      // stack: table_return_from_lua userdata peer table_return_from_lua
    }
    else
    {
        lua_pop(l, 1);                          // stack: table_return_from_lua userdata peer table_return_from_lua
    }

    lua_setmetatable(l, -2);                    // stack: table_return_from_lua userdata peer
    lua_setfenv(l, -2);                         // stack: table_return_from_lua userdata
    lua_pop(l, 2);
}

void ComponentLua::removeLuaTable()
{
    if (_succeedLoadingScript)
    {
        // LUA_REGISTRYINDEX["component"][strIndex] = nil
        
        lua_State *l = LuaEngine::getInstance()->getLuaStack()->getLuaState();
        
        lua_pushstring(l, KEY_COMPONENT);      // stack: "component"
        lua_rawget(l, LUA_REGISTRYINDEX);      // stack: LUA_REGISTRYINDEX["component"]
        lua_pushstring(l, _strIndex.c_str());  // stack: LUA_REGISTRYINDEX["component"] strIndex
        lua_pushnil(l);                        // stack: LUA_REGISTRYINDEX["component"] strIndex nil
        lua_rawset(l, -3);                     // stack: LUA_REGISTRYINDEX["component"]
    }
}

void ComponentLua::getUserData()
{
    lua_State *l = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    object_to_luaval<cocos2d::ComponentLua>(l, "cc.ComponentLua",this);
}

NS_CC_END

#include "ScriptingManager.h"

// Include from DontCrashEngine
#include "Resource.h"
#include "ApplicationLayer.h"

// Include from STL
#include <assert.h>

namespace DC_Engine
{
    ScriptingManager::ScriptingManager()
        : m_pState(nullptr, nullptr),
        m_stackSize(0)
    {
    }

    bool ScriptingManager::Init()
    {
        m_pState = std::unique_ptr<lua_State, decltype(&lua_close)>(luaL_newstate(), &lua_close);
        luaL_openlibs(m_pState.get());
        return true;
    }

    bool ScriptingManager::RunScript(std::shared_ptr<Resource> pResource)
    {
        bool error = luaL_loadbuffer(m_pState.get(),
            pResource->GetData().data(), pResource->GetData().size(), "script") ||
            lua_pcall(m_pState.get(), 0, 0, 0);

        if (error)
        {
            PrintLuaError("Script");
        }

        return !error;
    }

    bool ScriptingManager::RunScript(const std::string& fileName)
    {
        std::string filePath = ApplicationLayer::GetInstance()->GetBasePath();
        filePath += fileName;

        bool error = luaL_loadfile(m_pState.get(), filePath.c_str());
        if (error)
            PrintLuaError(filePath);

        return !error;
    }

    void ScriptingManager::CreateTable()
    {
        lua_newtable(m_pState.get());
        ++m_stackSize;
    }

    void ScriptingManager::AddToTable(const char* pKey)
    {
        lua_setfield(m_pState.get(), -2, pKey);
        --m_stackSize;
    }

    void ScriptingManager::AddToTable(const char* pKey, lua_CFunction pFunction)
    {
        lua_pushcfunction(m_pState.get(), pFunction);
        lua_setfield(m_pState.get(), -2, pKey);
    }

    void ScriptingManager::AddToTable(const char* pKey, void* pUserData)
    {
        lua_pushlightuserdata(m_pState.get(), pUserData);
        lua_setfield(m_pState.get(), -2, pKey);
    }

    void ScriptingManager::GetFromTable(const char* pKey)
    {
        lua_getfield(m_pState.get(), -1, pKey);
        ++m_stackSize;
    }

    void ScriptingManager::GetFromTable(int index)
    {
        lua_geti(m_pState.get(), -1, index);
        ++m_stackSize;
    }

    void ScriptingManager::GetGlobal(const char* pName)
    {
        lua_getglobal(m_pState.get(), pName);
        ++m_stackSize;
    }

    void ScriptingManager::SetGlobal(const char* pName)
    {
        lua_setglobal(m_pState.get(), pName);
        --m_stackSize;
    }

    void ScriptingManager::PushNumber(const double& number)
    {
        lua_pushnumber(m_pState.get(), number);
        ++m_stackSize;
    }

    void ScriptingManager::PushString(const char* pString)
    {
        lua_pushstring(m_pState.get(), pString);
        ++m_stackSize;
    }

    bool ScriptingManager::CallFunction(const int& numReturn)
    {
        if (lua_pcall(m_pState.get(), m_stackSize - 1, numReturn, 0) != 0)
        {
            PrintLuaError("Failed to call Function: ");
            m_stackSize = 0;
            return false;
        }

        m_stackSize = numReturn;
        return true;
    }

    void ScriptingManager::PopAll()
    {
        lua_pop(m_pState.get(), m_stackSize);
        assert(lua_gettop(m_pState.get()) == 0);
        m_stackSize = 0;
    }

    void ScriptingManager::PrintLuaError(const std::string& errorFile)
    {
        std::string errorMessage = "LuaScripting Error From Resourcefile: " + errorFile;
        errorMessage += "\n Error: ";
        std::string error = lua_tostring(m_pState.get(), -1);
        ApplicationLayer::GetInstance()->GetLog().LogError(errorMessage + error);
        lua_pop(m_pState.get(), 1);
    }

}

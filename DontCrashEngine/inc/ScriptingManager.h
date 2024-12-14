#pragma once

// Include from STL
#include <memory>
#include <string>

// Include from Lua
#include <lua.hpp>

#define LUA_GETGLUE(functionName, memberType, memberName) \
	memberType functionName() const { return m_##memberName; }

#define LUA_SETGLUE(functionName, memberType, memberName) \
	void functionName(memberType memberName) { m_##memberName = memberName; }

#define LUA_GETGLUE_NESTED(functionName, memberType, memberName, structMember) \
	memberType functionName() const { return m_##structMember.memberName; }

#define LUA_SETGLUE_NESTED(functionName, memberType, memberName, structMember) \
	void functionName(memberType memberName) { m_##structMember.memberName = memberName; }

#define LUA_TWO_PARAMETER_FUNCTIONGLUE(returnType, functionName, firstMemberType, firstMemberName, secondMemberType, secondMemberName) \
	returnType functionName(firstMemberType firstMemberName, secondMemberType secondMemberName);

#define LUA_REGISTER() \
	virtual void RegisterWithScript() override;

namespace DC_Engine
{
	class Resource;
	class ScriptingManager
	{
	// Functions
	public:
		ScriptingManager();

		bool Init();
		
		bool RunScript(std::shared_ptr<Resource> pResource);
		bool RunScript(const std::string& fileName);

		// Table Functions
		void CreateTable();
		void AddToTable(const char* pKey);
		void AddToTable(const char* pKey, lua_CFunction pFunction);
		void AddToTable(const char* pKey, void* pUserData);

		void GetFromTable(const char* pKey);
		void GetFromTable(int index);

		// Global data Functions
		void GetGlobal(const char* pName);
		void SetGlobal(const char* pName);

		// Push To stack
		void PushNumber(const double& number);
		void PushString(const char* pString);

		// Manage Lua Functions
		void StartFunction(const char* pFunction) { GetGlobal(pFunction); }
		bool CallFunction(const int& numReturn);

		void PopAll();

	private:
		void PrintLuaError(const std::string& errorFile);


	// Variables
	private:
		std::unique_ptr<lua_State, decltype(&lua_close)> m_pState;
		int m_stackSize;
	};
}


// Include from Utilites
#include <FileSystem.h>

// Include from STL
#include <string>
#include <vector>

// Debugging purpose
#include <iostream>
#include <filesystem>

// Unclude from Lua
#include <lua.hpp>

int main(int argc, char* args[])
{
	if (argc < 2)
		return 1;
	std::string path = args[1];
	const char* extension = ".h";
	std::vector<std::string> headerFiles = YS_Util::FileSystem::GetAllExtensionFiles(path, extension);

	// Initialize lua
	lua_State* pState = luaL_newstate();
	if (!pState)
		return 1;

	luaL_openlibs(pState);

	std::string luaBindingScript = path;
	luaBindingScript += "/GluaGen.lua";

	
	if (!std::filesystem::exists(luaBindingScript))
	{
		std::cout << std::filesystem::current_path() << std::endl;
		std::cout << "script not found: " << luaBindingScript << std::endl;
		lua_close(pState);
		return 1;
	}

	if (luaL_dofile(pState, luaBindingScript.c_str()) != LUA_OK)
	{
		std::cout << lua_tostring(pState, -1);
		lua_close(pState);
		return 1;
	}

	// Pass to the lua binder script
	for (const auto& headerFile : headerFiles)
	{
		lua_getglobal(pState, "HeaderFile");

		// Init _fileName for the headerFile object
		lua_getfield(pState, -1, "new");
		lua_pushvalue(pState, -2);
		lua_newtable(pState);

		if (lua_pcall(pState, 2, 1, 0))
		{
			std::cout << "Error: " << lua_tostring(pState, -1) << std::endl;
			lua_pop(pState, 1);
			continue;
		}

		lua_pushstring(pState, headerFile.c_str());
		lua_setfield(pState, -2, "_fileName");

		// Read the header file
		lua_getfield(pState, -1, "ReadFile");
		lua_pushvalue(pState, -2);
		if (lua_pcall(pState, 1, 0, 0) != LUA_OK)
		{
			std::cout << "Error: " << lua_tostring(pState, -1) << std::endl;
			lua_pop(pState, 1);
			continue;
		}

		// Write a file
		lua_getfield(pState, -1, "WriteGenFile");
		lua_pushvalue(pState, -2);
		if (lua_pcall(pState, 1, 0, 0) != LUA_OK)
		{
			std::cout << "Error: " << lua_tostring(pState, -1) << std::endl;
			lua_pop(pState, 1);
			continue;
		}
		lua_pop(pState, 1);

		lua_pop(pState, 1);

	}




	lua_close(pState);



	return 0;
}

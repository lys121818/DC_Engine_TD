kRegister = 'LUA_REGISTER'
kSetter = 'LUA_SETGLUE'
kGetter = 'LUA_GETGLUE'

GetGlue = {};
GetGlue._functionName = '';
GetGlue._memberName = '';
GetGlue._memberType = '';

function GetGlue:new(object)
	object = object or {};
	setmetatable(object, self);
	self.__index = self;
	return object;
end

function GetGlue:WriteToFile(file, class)
	file:write('	static int ', class._name, '_', self._functionName, '(lua_State* pState)\n');
	file:write('	{\n');
	file:write('		auto pClass = reinterpret_cast<', class._name, '*>(\n');
	file:write('			lua_touserdata(pState, 1));\n');
	file:write('		lua_pushnumber(pState, static_cast<', self._memberType,'>(pClass->', self._functionName, '()));\n');
	file:write('		return 1;\n');
	file:write('	}\n\n');
end

SetGlue = {};
SetGlue._functionName = '';
SetGlue._memberName = '';
SetGlue._memberType = '';

function SetGlue:new(object)
	object = object or {};
	setmetatable(object, self);
	self.__index = self;
	return object;
end

function SetGlue:WriteToFile(file, class)
	file:write('	static int ', class._name, '_', self._functionName, '(lua_State* pState)\n');
	file:write('	{\n');
	file:write('		auto pClass = reinterpret_cast<', class._name, '*>(\n');
	file:write('			lua_touserdata(pState, 1));\n');
	file:write('		', self._memberType, ' ', self._memberName, ' = static_cast<', self._memberType, '>(luaL_checknumber(pState,2));\n');
	file:write('		pClass->', self._functionName, '(', self._memberName, ');\n');
	file:write('		lua_pop(pState, 2);\n');
	file:write('		return 0;\n');
	file:write('	}\n\n');
end

function GetWords(line)
	local words = {};
	-- split by whitespace
	for word in line:gmatch("%S+") do
		-- split by non-alphanumeric
		for a in word:gmatch("[%w_]+") do
			table.insert(words,a);
		end
	end
	return words;
end 

kClassName = 'class';
Class = {};
Class._writeLuaBindings = false;
Class._name = '';
Class._registerFunctionBodyStart = {
	'{',
	'	auto& pGameState = DC_Engine::ApplicationLayer::GetInstance()->GetGame().GetCurrentState();',
	'	auto& scripting = pGameState.GetScriptingManager();',
	'	scripting.GetGlobal("g_sandbox");',
	'	scripting.GetFromTable("GameObjects");',
	'	scripting.GetFromTable(GetOwner()->GetId());',
	'	scripting.CreateTable();',
	'	scripting.AddToTable("this", this);',
	'	scripting.AddToTable("Move", Lua::TransformComponentMove);',
};

Class._registerFunctionBodyEnd = {
	'	scripting.PopAll();',
	'}',
};

function Class:new(object)
	object = object or {};
	object._getGlue = object._getGlue or {};
	object._setGlue = object._setGlue or {};
	object._writeLuaBindings = false;
	setmetatable(object, self);
	self.__index = self;
	return object;
end

function Class:Read(lines, startIndex)
	for lineIndex = startIndex + 1, #lines do
		local words = GetWords(lines[lineIndex]);
		for wordIndex = 1, #words do
			local word = words[wordIndex];
			if(word == kClassName) then
				return lineIndex -1;
			elseif (string.find(word, kRegister)) then
				self._writeLuaBindings = true;
			elseif (string.find(word, kGetter)) then
				local getGlue = GetGlue:new({
					_functionName = words[wordIndex + 1],
					_memberType = words[wordIndex + 2],
					_memberName = words[wordIndex + 3]
				});
				table.insert(self._getGlue, getGlue);
			elseif (string.find(word, kSetter)) then
				local setGlue = SetGlue:new({
					_functionName = words[wordIndex + 1],
					_memberType = words[wordIndex + 2],
					_memberName = words[wordIndex + 3],
				});
				table.insert(self._setGlue, setGlue);
			end
		end
	end
	return #lines;
end



HeaderFile = {};
HeaderFile._fileName = {};
HeaderFile._classes = {};


function HeaderFile:new(object)
    object = object or {}
    setmetatable(object, self)
    self.__index = self
    object._fileName = object._fileName or {}
    object._classes = object._classes or {}
    return object
end

function HeaderFile:ReadFile()
	-- Reset classes before reading a new file
	self._classes = {};

	-- Read files by line
	local lines = {};
	for line in io.lines(self._fileName) do
		table.insert(lines,line);
	end
	
	for lineIndex = 1, #lines do
		local words = GetWords(lines[lineIndex]);
		
		-- look for a class
		for wordIndex, word in ipairs(words) do
			-- only include actual class. exclude forward declare
			if(word == kClassName and words[wordIndex + 2] == 'public') then
				local class = Class:new({_name = words[wordIndex + 1]});
				lineIndex = class:Read(lines, lineIndex);
				table.insert(self._classes, class);
				break;
			end

		end
	end
end


function HeaderFile:WriteGenFile()
	-- error if the header files are empty
    if not self._fileName or self._fileName == "" then
        error("HeaderFile:WriteGenFile called with nil or empty _fileName")
    end

	local hasLuaBindings = false;
	for i, class in ipairs(self._classes) do
		if class._writeLuaBindings then
			hasLuaBindings = true;
			break;
		end
	end

	if not hasLuaBindings then
		return;
	end

	-- % is an secape character so we can fine the .
	local genFile = string.gsub(self._fileName, '%.h', '.gen');
	local file = io.open(genFile, 'w');

	-- write our intro
	file:write('namespace Lua\n');
	file:write('{\n');

	-- write our getters and setters
	for i, class in ipairs(self._classes) do
		for i, getGlue in ipairs(class._getGlue) do
			getGlue:WriteToFile(file, class);
		end

		for i, setGlue in ipairs(class._setGlue) do
			setGlue:WriteToFile(file, class);
		end
	end
	
	-- write our outro
	file:write('\n');	
	file:write('}\n\n');

	-- write our class register function
	for i, class in ipairs(self._classes) do
		file:write('void ', class._name, '::RegisterWithScript()\n');
		for i, line in ipairs(class._registerFunctionBodyStart) do
			file:write(line, '\n');
		end

		for i, getGlue in ipairs(class._getGlue) do
			file:write('	scripting.AddToTable("', getGlue._functionName, '", Lua::', class._name, '_', getGlue._functionName, ');\n');
		end

		for i, setGlue in ipairs(class._setGlue) do
			file:write('	scripting.AddToTable("', setGlue._functionName, '", Lua::', class._name, '_', setGlue._functionName, ');\n');
		end

		file:write('	scripting.AddToTable("', class._name, '");\n');

		for i, line in ipairs(class._registerFunctionBodyEnd) do
			file:write(line, '\n');
		end
	end

			

	file:close();
end
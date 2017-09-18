/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once
#include <string>

#include "Offsets.h"

namespace Moho
{
	typedef int Lua_State;

	class LuaObject
	{
	private:
		char pad_0x0000[0x1C]; //0x0000
	public:
		Lua_State* luaState; //0x001C 

		static LuaObject* GetRoot()
		{
			return *(LuaObject**)(Offsets::ADDR_GLOBALLUAOBJ);
		}

		char Execute(std::string script)
		{
			typedef char (__cdecl* LuaExecute_t)(LuaObject* state, const char *script_str);
			return reinterpret_cast<LuaExecute_t>(Offsets::FUNC_EXECUTELUA)(this, script.c_str());
		}

		char ExecuteFile(std::string filename);
	};
}
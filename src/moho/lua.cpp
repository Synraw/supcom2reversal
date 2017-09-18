/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#include "lua.h"

#include <fstream>
#include <streambuf>

char Moho::LuaObject::ExecuteFile(std::string filename)
{
	std::ifstream t(filename);
	std::string scripttxt;

	t.seekg(0, std::ios::end);
	scripttxt.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	scripttxt.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return Execute(scripttxt);
}
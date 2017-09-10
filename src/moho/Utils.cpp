/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#include "Utils.h"

std::string Moho::StrStripTags(const std::string &in)
{
	return in.substr(in.find_last_of('>') + 1);
}
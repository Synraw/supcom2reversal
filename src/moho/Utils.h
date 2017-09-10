/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

#include <string>

namespace Moho
{
	/*
		A lot of the strings that might be useful for front end display have some
		internal tags at the front as some sort of identifier. This function will
		clean the string leaving only the display part:
		Example input:  "<LOC UNIT_DESCRIPTION_0014>Mass Production Facility"
		Example output: "Mass Production Facility"
	*/
	std::string StrStripTags(const std::string &in);

}
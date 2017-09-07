/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

#include "MString.h"

namespace Moho
{
	class PlatoonTask;
	class PlatoonUnitGroup;
	class Unit;

	class CPlatoon
	{
	public:
	private:
		char pad_0x0000[0x38]; //0x0000
	public:
		PlatoonUnitGroup** m_pUnitGroupList; //0x0038 
		PlatoonUnitGroup** m_pUnitGroupListEnd; //0x003C 
	private:
		char pad_0x0040[0x2C]; //0x0040
	public:
		MString m_strOtherName; //0xE17888 
	private:
		char pad_0x0084[0x20]; //0x0084
	public:
		MString m_strName;
	private:
		char pad_0x00BC[0x44]; //0x00BC
	public:
		PlatoonTask* m_pPlatoonTask; //0x0100 
	private:
		char pad_0x0104[0x28]; //0x0104
		unsigned char N00000F15; //0x012C 
		unsigned char N00001CFF; //0x012D 
		unsigned char N00001D02; //0x012E 
		unsigned char N00001D00; //0x012F 
	private:
		char pad_0x0130[0x1D0]; //0x0130

	}; //Size=0x0300

	class PlatoonUnitGroup
	{
	private:
		char pad_0x0000[0x8]; //0x0000
	public:
		Unit** m_pUnits; //0x0008 
		Unit** m_pUnitsEnd;
	}; //Size=0x0058

	class PlatoonTask
	{
	private:
		char pad_0x0000[0x40]; //0x0000
	public:
		int m_iUnkn; //0x0040 
	private:
		char pad_0x0044[0x804]; //0x0044

	}; //Size=0x0848
}
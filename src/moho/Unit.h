/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

#include "MString.h"
#include "Math.h"

namespace Moho
{
	class UnitBlueprint;
	class SimArmy;
	class Platoon;

	class Unit
	{
	private:
		char pad_0x0000[0xB0]; //0x0000
	public:
		Vector4 m_vecPosition; //0x00B0 
	private:
		char pad_0x00C0[0xDC]; //0x00C0
	public:
		UnitBlueprint* m_pBlueprint; //0x019C 
	private:
		char pad_0x01A0[0x10]; //0x01A0
	public:
		SimArmy* m_pParentArmy; //0x01B0 
	private:
		char pad_0x01B4[0x34]; //0x01B4
	public:
		void* N000010ED; //0x01E8 
	private:
		char pad_0x01EC[0x69C]; //0x01EC
	public:
		Platoon* m_pPlatoon; //0x0888 
	private:
		char pad_0x088C[0x120]; //0x088C
	public:
		unsigned char N000015F9; //0x09AC 
		unsigned char m_bIsEngineer; //0x09AD 
		unsigned char m_bIsNaval; //0x09AE 
		unsigned char N000016B0; //0x09AF 
	private:
		char pad_0x09B0[0x2D0]; //0x09B0

	public:

		Unit* FixPointer()
		{
			return (Unit*)((unsigned int)this - 8);
		}

	}; //Size=0x0C80

	class UnitBlueprint
	{
	private:
		char pad_0x0000[0xC]; //0x0000
	public:
		MString m_strInternalName; //0x000C 
	private:
		char pad_0x0024[0x4]; //0x0024
	public:
		MString m_strDescription;
	private:
		char pad_0x0040[0x4]; //0x0040
	public:
		MString m_szUnitBPFile; //0x0044 
	private:
		char pad_0x005C[0x100]; //0x005C
	public:
		MString m_strIconName;
	private:
		char pad_0x0174[0x2B4]; //0x0174
	public:
		MString m_strUnitType;
	private:
		char pad_0x0440[0x44]; //0x0440
	public:
		MString m_strUnitName;
	};
}
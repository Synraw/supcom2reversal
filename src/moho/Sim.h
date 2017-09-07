/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

namespace Moho
{
	class MapRelated;
	class SimArmy;

	class Sim
	{
	private:
		char pad_0x0000[0x8D0]; //0x0000
	public:
		MapRelated* m_pMapRelated; //0x08D0 
	private:
		char pad_0x08D4[0x8]; //0x08D4
		void* somethingLuaRelated; //0x08DC I'll figure this out later but its references a lot
		char pad_0x08E0[0x38]; //0x08E0
	public:
		int m_iBeatNumber; //0x0918 
	private:
		char pad_0x091C[0x4]; //0x091C
	public:
		int m_iTickNumber; //0x0920 
	private:
		char pad_0x0924[0x14]; //0x0924
	public:
		SimArmy** m_pArmyList; //0x0938 
		SimArmy** m_pArmyListEnd; //0x093C 
	};

	class MapInfo
	{
	private:
		char pad_0x0000[0x4]; //0x0000
	public:
		int m_iWidth; //0x0004 
		int m_iHeight; //0x0008 
	private:
		char pad_0x000C[0x18]; //0x000C

	}; //Size=0x0024

	class HeightField
	{
	private:
		char pad_0x0000[0x44]; //0x0000

	}; //Size=0x0044

	class MapRelated
	{
	public:
		MapInfo* m_pMapInfo; //0x0000 
		HeightField* m_pHeightField; //0x0004 
	private:
		char pad_0x0008[0x3C]; //0x0008
	}; //Size=0x0044
}
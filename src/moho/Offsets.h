/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

namespace Moho
{
	namespace Offsets
	{
		constexpr unsigned int ADDR_SIMDRIVER  = 0x18F0698; // Near " Do Loading 5" - Pattern A3 ? ? ? ? 3B CF
		constexpr unsigned int ADDR_DEVICED3D9 = 0x18F3238; // One func up of Direct3DCreate9 call is full of references - Pattern 8B 3D ? ? ? ? 8B 6C 24 44
		constexpr unsigned int ADDR_WORLDMAP   = 0x18CA5C0;
		constexpr unsigned int ADDR_CAMERALIST = 0x193477C;
		constexpr unsigned int ADDR_RENDERVIEWPORT = 0x18CA588;
		constexpr unsigned int ADDR_GLOBALLUAOBJ   = 0x18CA654;

		constexpr unsigned int FUNC_EXECUTELUA = 0x00501E00;
	}
}
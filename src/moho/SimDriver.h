/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

#include "Offsets.h"

namespace Moho
{
	class Sim;
	class ClientManager;

	class SimDriver
	{
	private:
		char pad_0x0000[0x4]; //0x0000
	public:
		Sim* m_pSim; //0x0004 
		ClientManager* m_pClientManager; //0x0008

		static SimDriver* GetInstance()
		{
			return *(SimDriver**)(Offsets::ADDR_SIMDRIVER);
		}
	};
}
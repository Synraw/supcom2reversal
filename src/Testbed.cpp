/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/

#include "Testbed.h"

#include <cstdio>
#include <iostream>
#include <Windows.h>

// Used for unloading ourself
HMODULE thisMod = 0;

/*
	Initial setup when we first load
	This function is ran as its own thread from DLLMain
*/
DWORD WINAPI InitialThread(LPVOID lpThreadParameter)
{
	// Attach a console to the game to output anything we might need
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	std::printf("Testbed for %s SDK ( version %s )\n", Moho::GAME_NAME, Moho::GAME_VERSION);

	// Test access to the engine
	Moho::SimDriver* simDriver = Moho::SimDriver::GetInstance();

	if (simDriver != nullptr)
	{
		for (auto army : simDriver->m_pSim->m_armyList)
		{
			std::cout << "Army owned by " << army->m_strPlayerName.get() << " is of faction " << army->GetFactionName() << std::endl;
			for (auto platoon : army->m_platoonList)
			{
				std::cout << "\tPlatoon " << platoon->m_strName.get() << "(" << platoon->m_strOtherName.get() 
					<< ") has " << platoon->m_unitGroupList.size() << " unit groups\n";

				for (auto tempunit : platoon->m_unitGroupList[0]->m_units)
				{
					Moho::Unit* pUnit = tempunit->FixPointer();

					std::cout << "\t\tUnit " << pUnit->m_pBlueprint->m_strDisplayName.get() << " - " << pUnit->m_pBlueprint->m_strDescription.get() << std::endl;
				}
			}
		}
	}

	FreeLibraryAndExitThread(thisMod, 0);
	return 0;
}

/*
	Entry point to this application
	On Attach, creates a new thread to handle setup related tasks as api calls in here aren't safe
	On Detach, clean up any hooks still in place in the game so we can unload without affecting the game
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		thisMod = hinstDLL;
		CreateThread(NULL, NULL, InitialThread, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		// add when ready
		break;
	}

	return TRUE;
}
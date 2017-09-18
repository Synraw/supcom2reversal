/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/

#include "Testbed.h"
#include "Renderer.h"
#include "Memory.h"

#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <detours.h>


// Used for unloading ourself
HMODULE thisMod = 0;

// Rendering related
typedef HRESULT(__stdcall* Present) (LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef HRESULT(__stdcall* Reset) (LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
HRESULT __stdcall mPresent(LPDIRECT3DDEVICE9 Device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
HRESULT __stdcall mReset(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* p);
Present oPresent;
Reset oReset;
Memory::VMTManager DirectXVMTManager;

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
	Moho::DeviceD3D9* d3d9Render = Moho::DeviceD3D9::GetInstance();

	if (simDriver == nullptr)
	{
		while (simDriver == nullptr)
		{
			simDriver = Moho::SimDriver::GetInstance();
			Sleep(2000);
		}
	}

	while(d3d9Render == nullptr)
		d3d9Render = Moho::DeviceD3D9::GetInstance();

	while (d3d9Render->m_pDevice == nullptr)
	{
		Sleep(200);
	}

	printf("Game Ready\n");

	// Set up our drawing related stuff
	DirectXVMTManager.Initialise((DWORD*)d3d9Render->m_pDevice);
	oPresent = (Present)DirectXVMTManager.HookMethod((DWORD)mPresent, 17);;
	oReset = (Reset)DirectXVMTManager.HookMethod((DWORD)mReset, 16);

	return 0;
}

struct ScreenRect
{
	float x, y, w, h;
};

bool GetBox(Moho::Unit* pEntity, ScreenRect &result)
{
	// Variables
	Moho::CollisionAABB aabb;
	Moho::Vector3 vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	pEntity->m_pCollisionPrim->GetCollisionExtents(&aabb);
	

	// Points of a 3d bounding box
	Moho::Vector3 points[] = { Moho::Vector3{aabb.m_vecMin.x, aabb.m_vecMin.y, aabb.m_vecMin.z},
		Moho::Vector3{ aabb.m_vecMin.x, aabb.m_vecMax.y, aabb.m_vecMin.z},
		Moho::Vector3{ aabb.m_vecMax.x, aabb.m_vecMax.y, aabb.m_vecMin.z},
		Moho::Vector3{ aabb.m_vecMax.x, aabb.m_vecMin.y, aabb.m_vecMin.z},
		Moho::Vector3{ aabb.m_vecMax.x, aabb.m_vecMax.y, aabb.m_vecMax.z},
		Moho::Vector3{ aabb.m_vecMin.x, aabb.m_vecMax.y, aabb.m_vecMax.z},
		Moho::Vector3{ aabb.m_vecMin.x, aabb.m_vecMin.y, aabb.m_vecMax.z},
		Moho::Vector3{ aabb.m_vecMax.x, aabb.m_vecMin.y, aabb.m_vecMax.z} };


	// Get screen positions
	if (!Moho::WorldToScreen(points[3], flb) || !Moho::WorldToScreen(points[5], brt)
		|| !Moho::WorldToScreen(points[0], blb) || !Moho::WorldToScreen(points[4], frt)
		|| !Moho::WorldToScreen(points[2], frb) || !Moho::WorldToScreen(points[1], brb)
		|| !Moho::WorldToScreen(points[6], blt) || !Moho::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Moho::Vector3 arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

/*
	Drawing Happens in here
*/
HRESULT __stdcall mPresent(LPDIRECT3DDEVICE9 Device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	Renderer* r = Renderer::Get();
	static bool firstRun = false;
	if (!firstRun)
	{
		firstRun = true;
		r->Init(Device);
	}

	r->BeginDraw();
	//-------------------------------------------------------

	r->Text(10, 10, Renderer::FontMain, D3DCOLOR_RGBA(255, 255, 255, 170), Moho::GAME_NAME);

	Moho::SimDriver* simDriver = Moho::SimDriver::GetInstance();

	if (simDriver != nullptr)
	{
		static bool last = false;
		bool thistime = GetAsyncKeyState(VK_DOWN);
		if (last == false && thistime)
		{
			if (simDriver->m_pSim->luaObject->ExecuteFile("dump.lua"))
				printf("[DEBUG] Script Succeeded\n");
			else
				printf("[DEBUG] Syntax Error\n");
		}

		last = thistime;

		for (auto army : simDriver->m_pSim->m_armyList)
		{
			if (army == nullptr) continue;

			for (auto platoon : army->m_platoonList)
			{
				if (platoon == nullptr || platoon->m_unitGroupList.begin() == nullptr || platoon->m_unitGroupList.size() < 1)
					continue;

				for (auto tempunit : platoon->m_unitGroupList[0]->m_units)
				{
					Moho::Unit* pUnit = tempunit->FixPointer();
					if (tempunit && pUnit->m_pBlueprint && pUnit->m_pBlueprint->m_strUnitName.length() > 0)
					{
						D3DXVECTOR3 result; ScreenRect box;
						if (Moho::WorldToScreen(Moho::Vector3(pUnit->m_vecPosition.x, pUnit->m_vecPosition.y, pUnit->m_vecPosition.z), result))
						{
							if (pUnit->m_pCollisionPrim && GetBox(pUnit, box))
							{
								// Text centering stuff
								std::string strUnitName = Moho::StrStripTags(pUnit->m_pBlueprint->m_strUnitName.get());
								int width = r->GetTextWidth(Renderer::FontESP, strUnitName.c_str());

								r->Outline(box.x, box.y, box.w, box.h, army->m_argbTeamColour);
								r->Outline(box.x-1, box.y-1, box.w+2, box.h+2, D3DCOLOR_ARGB(255, 0, 0, 0));
								r->Outline(box.x+1, box.y+1, box.w-2, box.h-2, D3DCOLOR_ARGB(255, 0, 0, 0));

								// Text w/ shadow
								r->Text(box.x + (box.w/2) - (width/2) + 1, box.y - 15, Renderer::FontESP, D3DCOLOR_ARGB(255,0,0,0), strUnitName.c_str());
								r->Text(box.x + (box.w / 2) - (width / 2), box.y - 16, Renderer::FontESP, army->m_argbTeamColour, strUnitName.c_str());

							}
							else
							{
								r->Text(result.x, result.y, Renderer::FontESP, army->m_argbTeamColour, Moho::StrStripTags(pUnit->m_pBlueprint->m_strUnitName.get()).c_str());
							}
						}
					}
				}
			}
		}
	}

	//-------------------------------------------------------
	r->EndDraw();
	return oPresent(Device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

/*
	Reset our renderer when the game does
*/
HRESULT __stdcall mReset(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* p)
{
	Renderer::Get()->OnResetDevice();
	printf("Reset\n");
	return oReset(Device, p);
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
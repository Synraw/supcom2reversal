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

// Used for unloading ourself
HMODULE thisMod = 0;

// World to Screen stuff
D3DXMATRIX   viewprojection;
D3DVIEWPORT9 viewport;

// Rendering related
typedef HRESULT(__stdcall* Present) (LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef HRESULT(__stdcall* Reset) (LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT(__stdcall* DrawIndexedPrimitive)(LPDIRECT3DDEVICE9 pDev, D3DPRIMITIVETYPE Type, INT Base, UINT Min, UINT Num, UINT Start, UINT Prim);
HRESULT __stdcall mPresent(LPDIRECT3DDEVICE9 Device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
HRESULT __stdcall mReset(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* p);
Present oPresent;
Reset oReset;
DrawIndexedPrimitive oDIP;
Memory::VMTManager DirectXVMTManager;

/*
 Temporary hook to grab the View Projection matrix from the shader constants when rendering the game models
 Will remove once I find where the game is storing the matrix
*/
HRESULT WINAPI nDrawIdP(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT Base, UINT Min, UINT Num, UINT Start, UINT Prim)
{
	LPDIRECT3DVERTEXBUFFER9 Stream_Data;
	UINT Offset = 0;
	UINT Stride = 0;

	if (pDevice->GetStreamSource
	(0, &Stream_Data, &Offset, &Stride) == S_OK)
		Stream_Data->Release();

	// 36 seems to be world objects
	if (Stride == 36)
	{
		pDevice->GetVertexShaderConstantF(7, viewprojection, 4);
	}

	return oDIP(pDevice, Type, Base, Min, Num, Start, Prim);
}

bool WorldToScreen(const D3DXVECTOR3& WorldPos, D3DXVECTOR3* ScreenPos)
{
	D3DXMATRIXA16 viewProj, identity;
	D3DXMatrixIdentity(&identity);
	D3DXMatrixTranspose (&viewProj, &viewprojection);
	D3DXVec3Project(ScreenPos, &WorldPos, &viewport, &viewProj, &identity, &identity);
	return ScreenPos->z < 1.0f;
}

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

	// Set up our drawing related stuff
	Renderer::Get()->Init(d3d9Render->m_pDevice);
	DirectXVMTManager.Initialise((DWORD*)d3d9Render->m_pDevice);
	oPresent = (Present)DirectXVMTManager.HookMethod((DWORD)&mPresent, 17);
	oReset = (Reset)DirectXVMTManager.HookMethod((DWORD)&mReset, 16);
	oDIP = (DrawIndexedPrimitive)DirectXVMTManager.HookMethod((DWORD)&nDrawIdP, 82);

	return 0;
}

/*
	Drawing Happens in here
*/
HRESULT __stdcall mPresent(LPDIRECT3DDEVICE9 Device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	Renderer* r = Renderer::Get();
	r->BeginDraw();
	//-------------------------------------------------------

	r->Text(10, 10, Renderer::FontMain, D3DCOLOR_RGBA(255, 255, 255, 170), Moho::GAME_NAME);

	Device->GetViewport(&viewport);

	Moho::SimDriver* simDriver = Moho::SimDriver::GetInstance();

	if (simDriver != nullptr)
	{
		for (auto army : simDriver->m_pSim->m_armyList)
		{
			for (auto platoon : army->m_platoonList)
			{
				for (auto tempunit : platoon->m_unitGroupList[0]->m_units)
				{
					Moho::Unit* pUnit = tempunit->FixPointer();
					if (pUnit && pUnit->m_pBlueprint && pUnit->m_pBlueprint->m_strUnitName.length() > 0)
					{
						D3DXVECTOR3 result;
						if (WorldToScreen(D3DXVECTOR3(pUnit->m_vecPosition.x, pUnit->m_vecPosition.y, pUnit->m_vecPosition.z), &result))
						{
							r->Text(result.x, result.y, Renderer::FontESP, army->m_argbTeamColour, Moho::StrStripTags(pUnit->m_pBlueprint->m_strUnitName.get()).c_str());
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
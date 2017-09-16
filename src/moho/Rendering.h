/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "Offsets.h"

namespace Moho
{
	class DeviceD3D9
	{
	public:
		static DeviceD3D9* GetInstance()
		{
			return *(DeviceD3D9**)(Offsets::ADDR_DEVICED3D9);
		}

	private:
		char pad_0x0000[0x98]; //0x0000
	public:
		LPDIRECT3D9 m_pD3D; //0x0098 
		LPDIRECT3DDEVICE9 m_pDevice; //0x009C 
	};

	class RenViewport
	{
	public:
		static RenViewport* GetInstance()
		{
			return *(RenViewport**)(Offsets::ADDR_RENDERVIEWPORT);
		}
	private:
		char pad_0x0000[0x180]; //0x0000
	public:
		int m_iScreenWidth; //0x0180 
		int m_iScreenHeight; //0x0184 
	}; //Size=0x01A0

}
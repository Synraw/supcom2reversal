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
#include "MString.h"
#include "MArray.h"
#include "Math.h"

namespace Moho
{
	class CameraImpl
	{
	public:
	private:
		char pad_0x0000[0x5CC]; //0x0000
	public:
		MString m_strCameraName; //0x05CC 
	private:
		char pad_0x05E4[0x60]; //0x05E4
	public:
		D3DXMATRIX m_matView; //0x0644 
		D3DXMATRIX m_matViewProj; //0x0684 
	}; //Size=0x0858

	class CameraList
	{
	private:

		enum ECAMIndexes
		{
			CAM_WORLD,
			CAM_RADAR,
		};

		MArray<CameraImpl*> m_cameraList;
	public:

		static CameraList* GetInstance()
		{
			return (CameraList*)0x193477C;
		}

		CameraImpl* GetWorldCamera()
		{
			if (m_cameraList.size() > 0)
				return m_cameraList[CAM_WORLD];
			return nullptr;
		}

		CameraImpl* GetRadarCamera()
		{
			if (m_cameraList.size() > 1)
				return m_cameraList[CAM_RADAR];
			return nullptr;
		}
	};

	/*
	Projects a given world point to screen coordinates using the current world camera
	*/
	bool WorldToScreen(const Vector3& in, Vector3& out);
}
/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#include "Camera.h"

#include "Rendering.h"
#include <d3d9.h>
#include <d3dx9.h>

bool Moho::WorldToScreen(const Moho::Vector3& in, Moho::Vector3& out)
{
	D3DXMATRIXA16 identity;

	Moho::RenViewport* viewport = Moho::RenViewport::GetInstance();
	Moho::CameraList* camlist = Moho::CameraList::GetInstance();
	Moho::CameraImpl* camera = camlist->GetWorldCamera();

	if (camlist == nullptr || camera == nullptr || viewport == nullptr) return false;

	D3DXMatrixIdentity(&identity);
	D3DVIEWPORT9 vp;
	vp.MaxZ = 1.f;
	vp.MinZ = -1.f;
	vp.X = 0.f;
	vp.Y = 0.f;
	vp.Width = viewport->m_iScreenWidth;
	vp.Height = viewport->m_iScreenHeight;

	D3DXVec3Project(&out, &in, &vp, &camera->m_matViewProj, &identity, &identity);
	return  true;// out.z < 0.f;
}

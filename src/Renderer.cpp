#include "Renderer.h"

#include <stdio.h>

Renderer* Renderer::instance = 0;

DWORD Renderer::Vertex3D::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

void Renderer::Init(LPDIRECT3DDEVICE9 Device)
{
	this->Device = Device;
	Device->CreateStateBlock(D3DSBT_ALL, &stateBlockOld);
	
	D3DVIEWPORT9 vp;
	Device->GetViewport(&vp);
	Screen.w = float(vp.Width);
	Screen.h = float(vp.Height);
	Screen.cx = Screen.w / 2;
	Screen.cy = Screen.h / 2;

	D3DXCreateSprite(Device, &pSprite);
	D3DXCreateFont(Device, 18, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Tahoma", &Fonts[FontMain]);
	D3DXCreateFont(Device, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Tahoma", &Fonts[FontConsole]);
	D3DXCreateFont(Device, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Tahoma", &Fonts[FontESP]);
	D3DXCreateFont(Device, 24, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_SWISS, "Courier New", &Fonts[FontBig]);
	D3DXCreateFont(Device, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial", &Fonts[FontTab]);

	for (int i = 0; i < MAX_FONT_ID; i++)
	{
		if (!Fonts[i])
		{
			printf("FAILED TO CREATE FONT!");
			D3DXCreateFont(Device, 18, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial", &Fonts[i]);
		}
	}
	
}

void Renderer::BeginDraw()
{
	stateBlockOld->Capture();
	
	Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	Device->SetVertexShader(nullptr);
	Device->SetPixelShader(nullptr);

	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	Device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	
	Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	D3DVIEWPORT9 vp;
	Device->GetViewport(&vp);

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	Device->SetTransform(D3DTS_WORLD, &mat);
	Device->SetTransform(D3DTS_VIEW, &mat);
	D3DXMatrixOrthoLH(&mat, float( vp.Width ), float( vp.Height ), -1.0f, 1.0f); // SET VIEWPORT SIZE IN HERE
	Device->SetTransform(D3DTS_PROJECTION, &mat);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Renderer::EndDraw()
{
	if (DrawSprite)
	{
		pSprite->Flush();
		DrawSprite = false;
	}

	pSprite->End();

	stateBlockOld->Apply();
}

void Renderer::OnResetDevice()
{
	D3DVIEWPORT9 vp;
	Device->GetViewport(&vp);
	Screen.w = float(vp.Width);
	Screen.h = float(vp.Height);
	Screen.cx = Screen.w / 2;
	Screen.cy = Screen.h / 2;

	if(pSprite)
		pSprite->OnResetDevice();

	for (int i = 0; i < MAX_FONT_ID; i++)
		Fonts[i]->OnResetDevice();
}

void Renderer::OnLostDevice()
{
	if (pSprite)
		pSprite->OnLostDevice();
	
	for (int i = 0; i < MAX_FONT_ID; i++)
		Fonts[i]->OnLostDevice();
}

void Renderer::Clear(float x, float y, float w, float h, D3DCOLOR color)
{
	if (DrawSprite)
	{
		pSprite->Flush();
		DrawSprite = false;
	}

	Vertex3D vert[4] =
	{
		{ D3DXVECTOR3(x, y, 0.0f), 1.0f, color },
		{ D3DXVECTOR3(x + w, y, 0.0f), 1.0f, color },
		{ D3DXVECTOR3(x, y + h, 0.0f), 1.0f, color },
		{ D3DXVECTOR3(x + w, y + h, 0.0f), 1.0f, color }
	};

	Device->SetTexture(0, nullptr);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Device->SetVertexShader(nullptr);
	Device->SetFVF(Vertex3D::FVF);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vert, sizeof(Vertex3D));
}

void Renderer::ClearGradientHorizontal(float x, float y, float w, float h, D3DCOLOR color_left, D3DCOLOR color_right)
{
	if (DrawSprite)
	{
		pSprite->Flush();
		DrawSprite = false;
	}

	Vertex3D vert[4] =
	{
		{ D3DXVECTOR3(x, y, 0.0f), 1.0f, color_left },
		{ D3DXVECTOR3(x + w, y, 0.0f), 1.0f, color_right },
		{ D3DXVECTOR3(x, y + h, 0.0f), 1.0f, color_left },
		{ D3DXVECTOR3(x + w, y + h, 0.0f), 1.0f, color_right }
	};

	Device->SetTexture(0, nullptr);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Device->SetVertexShader(nullptr);
	Device->SetFVF(Vertex3D::FVF);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vert, sizeof(Vertex3D));
}

void Renderer::TestTriangle(float x, float y, float size, D3DCOLOR color)
{
	if (DrawSprite)
	{
		pSprite->Flush();
		DrawSprite = false;
	}

	Vertex3D vert[4] =
	{
		{ D3DXVECTOR3(x + size, y, 0.0f), 1.0f, color },
		{ D3DXVECTOR3(x + size, y + size, 0.0f), 1.0f, color },
		{ D3DXVECTOR3(x, y + size, 0.0f), 1.0f, color },
	};

	Device->SetTexture(0, nullptr);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Device->SetVertexShader(nullptr);
	Device->SetFVF(Vertex3D::FVF);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, vert, sizeof(Vertex3D));
}

void Renderer::TestTriangle2(float x, float y, float size, D3DCOLOR color)
{
	if (DrawSprite)
	{
		pSprite->Flush();
		DrawSprite = false;
	}

	Vertex3D vert[4] =
	{
		{ D3DXVECTOR3(x , y, 0.0f), 1.0f, color },
		{ D3DXVECTOR3(x + size, y, 0.0f), 1.0f, color },
		{ D3DXVECTOR3(x + size, y + size, 0.0f), 1.0f, color },
	};

	Device->SetTexture(0, nullptr);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Device->SetVertexShader(nullptr);
	Device->SetFVF(Vertex3D::FVF);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, vert, sizeof(Vertex3D));
}


void Renderer::Outline(float x, float y, float w, float h, D3DCOLOR color)
{
	Line(x, y, x+w, y, color); // Top
	Line(x, y, x, y+h, color); // Left
	Line(x, y+h, x+w, y+h, color); // Bottom
	Line(x+w, y, x+w, y+h, color); // Right

}

void Renderer::Line(float x, float y, float x2, float y2, D3DCOLOR Color)
{
	if (DrawSprite)
	{
		pSprite->Flush();
		DrawSprite = false;
	}

	Vertex3D Vert[2] =
	{
	{ D3DXVECTOR3((float)x, (float)y, 0.0f), 1.0f, Color },
	{ D3DXVECTOR3((float)x2, (float)y2, 0.0f), 1.0f, Color }
	};

	Device->SetTexture(0, nullptr);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Device->SetVertexShader(nullptr);
	Device->SetFVF(Vertex3D::FVF);

	Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, Vert, sizeof(Vertex3D));
}

int Renderer::Text(float x, float y, FontIDs font, D3DCOLOR dColor, const char* szString, ...)
{
	DrawSprite = true;

	CHAR szBuffer[101] = "";

	va_list ArgumentList;
	va_start(ArgumentList, szString);
	vsnprintf_s(szBuffer, sizeof(szBuffer), sizeof(szBuffer) - strlen(szBuffer), szString, ArgumentList);
	va_end(ArgumentList);

	RECT Rect = { x, y, x + 500, y + 50 };

	return Fonts[font]->DrawText(pSprite, szBuffer, -1, &Rect, DT_NOCLIP, dColor);
}

int Renderer::Text(float x, float y, FontIDs font, D3DCOLOR dColor, const wchar_t* szString)
{
	RECT Rect = { x, y, x + 500, y + 50 };

	return Fonts[font]->DrawTextW(pSprite, szString, -1, &Rect, DT_NOCLIP, dColor);
}

int Renderer::GetTextWidth(FontIDs font, const char* szString, ...)
{
	CHAR szBuffer[101] = "";

	va_list ArgumentList;
	va_start(ArgumentList, szString);
	vsnprintf_s(szBuffer, sizeof(szBuffer), sizeof(szBuffer) - strlen(szBuffer), szString, ArgumentList);
	va_end(ArgumentList);

	RECT oRect = { 0, 0, 500, 50 };
	Fonts[font]->DrawText(NULL, szBuffer, -1, &oRect, DT_CALCRECT, NULL);

	return oRect.right;
}

int Renderer::GetTextWidth(FontIDs font, const wchar_t* szString)
{
	RECT oRect = { 0, 0, 500, 50 };
	Fonts[font]->DrawTextW(NULL, szString, -1, &oRect, DT_CALCRECT, NULL);
	return oRect.right;
}

RECT Renderer::GetTextSize(FontIDs font, const char* szString, ...)
{
	CHAR szBuffer[101] = "";

	va_list ArgumentList;
	va_start(ArgumentList, szString);
	vsnprintf_s(szBuffer, sizeof(szBuffer), sizeof(szBuffer) - strlen(szBuffer), szString, ArgumentList);
	va_end(ArgumentList);

	RECT oRect = { 0, 0, 500, 50 };
	Fonts[font]->DrawText(NULL, szBuffer, -1, &oRect, DT_CALCRECT, NULL);

	return oRect;
}

RECT Renderer::GetTextSize(FontIDs font, const wchar_t* szString)
{
	RECT oRect = { 0, 0, 500, 50 };
	Fonts[font]->DrawTextW(NULL, szString, -1, &oRect, DT_CALCRECT, NULL);

	return oRect;
}
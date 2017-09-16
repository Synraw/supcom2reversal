#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

//
// Render class for dx9
//
class Renderer
{
public:

	struct Vertex3D
	{
		D3DXVECTOR3 Pos;
		float fRhw;
		D3DCOLOR d3Diffuse;

		static DWORD FVF;
	};

	enum FontIDs
	{
		FontMain,
		FontESP,
		FontConsole,
		FontBig,
		FontTab,
		MAX_FONT_ID
	};

	inline static Renderer* Get() { if (instance == nullptr) instance = new Renderer();  return instance; }

	// Setup shit
	void Init(LPDIRECT3DDEVICE9 Device);

	// State blocks and shit
	void BeginDraw();
	void EndDraw();

	//D3D shit
	void OnResetDevice();
	void OnLostDevice();

	// Normal Drawing functions
	void Clear(float x, float y, float w, float h, D3DCOLOR color);
	void ClearGradientHorizontal(float x, float y, float w, float h, D3DCOLOR color_left, D3DCOLOR color_right);

	void TestTriangle(float x, float y, float size, D3DCOLOR color);
	void TestTriangle2(float x, float y, float size, D3DCOLOR color);

	void Outline(float x, float y, float w, float h, D3DCOLOR color);
	void Line(float x, float y, float x2, float y2, D3DCOLOR Color);

	// Text
	int Text(float x, float y, FontIDs font, D3DCOLOR dColor, const char* szString, ...);
	int Text(float x, float y, FontIDs font, D3DCOLOR dColor, const wchar_t* szString);
	int GetTextWidth(FontIDs font, const char* szString, ...);
	int GetTextWidth(FontIDs font, const wchar_t* szString);

	RECT GetTextSize(FontIDs font, const char* szString, ...);
	RECT GetTextSize(FontIDs font, const wchar_t* szString);

	bool DrawSprite;

	struct
	{
		float w, h;
		float cx, cy;
	} Screen;

private:
	Renderer(Renderer const&) {};
	Renderer() { instance = this; }
	Renderer& operator=(Renderer const&) {};
	~Renderer() {  };

	LPDIRECT3DDEVICE9 Device;
	LPD3DXSPRITE pSprite;

	LPD3DXFONT Fonts[MAX_FONT_ID];
	
	IDirect3DStateBlock9* stateBlockOld;
	IDirect3DStateBlock9* stateBlockNew;


protected:
	static Renderer* instance;


};
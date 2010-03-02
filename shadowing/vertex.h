#pragma once
	const int Alpha = 60;
	const DWORD RED = D3DCOLOR_ARGB(Alpha,255, 0, 0);
	const DWORD BLACK = D3DCOLOR_ARGB(Alpha, 0, 0, 0);
	const DWORD MAGENTA = D3DCOLOR_ARGB(Alpha, 255, 0, 255);
	const DWORD YELLOW = D3DCOLOR_ARGB(Alpha, 255, 255, 0);
	const DWORD GREEN = D3DCOLOR_ARGB(Alpha, 0, 255, 0);
	const DWORD WHITE = D3DCOLOR_ARGB(Alpha, 255, 255, 255);
	const DWORD CYAN = D3DCOLOR_ARGB(Alpha, 0, 255, 255);	
	const DWORD BLUE = D3DCOLOR_ARGB(Alpha, 0, 0, 255);
	const DWORD BACKGROUND = D3DCOLOR_ARGB(Alpha, 0, 100, 110);

	struct SKINNINGVERTEX		
	{
		float x, y, z;
		D3DXVECTOR4 normal;
		DWORD color;
		float w1, w2; 
		
		SKINNINGVERTEX(float x = 0.0f, float y = 0.0f, float z = 0.0f,
					 DWORD color = RED, float w1 = 0.0f, float w2 = 0.0f):
					x(x), y(y), z(z),normal(normal), color(color), w1(w1), w2(w2) {}
	};
	struct CUSTOMVERTEX		
	{
		float x, y, z;
		D3DXVECTOR4 normal;
		DWORD color;
		CUSTOMVERTEX(float x = 0.0f, float y = 0.0f, float z = 0.0f, 
					 DWORD color = RED):
					x(x), y(y), z(z), color(color){}
		CUSTOMVERTEX(float x, float y, float z, D3DXVECTOR4 normal, 
		 DWORD color):
		x(x), y(y), z(z), normal(normal), color(color){D3DXVec4Normalize(&normal, &normal);}
	};
	typedef unsigned short CUSTOMINDEX;

const D3DVERTEXELEMENT9 VertexDeclaration[] = 
{
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
D3DDECL_END()
};
const D3DVERTEXELEMENT9 SkinningDeclaration[] =
{
    {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
    {0, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    {0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()
};
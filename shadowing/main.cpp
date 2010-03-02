#include <windows.h>
#include <stdlib.h>
#include "graphics.h"
#include "models.h"

D3D::Graphics device;
MyCylinder::Cylinder cylinder;
MyPyramid::Pyramid pyramid;
MyPlane::Plane plane;
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();
const int SCREENWIDTH = 700;
const int SCREENHEIGHT = 700;
const float deltaR = 0.1f;
const float deltaPhi = D3DX_PI/20;
const float deltaTetha = D3DX_PI/30;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR, int )
{
	HANDLE ghInstance;
	WNDCLASS wcex;
	HWND hwnd;
	TCHAR WindowTitle[] = _T("Lighting");
    TCHAR WindowClassName[] = _T("AppClass");

	if (!hPrevInstance)
	{
		wcex.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.lpszClassName = WindowClassName;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
		wcex.lpszMenuName = 0;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;

		if (!RegisterClass(&wcex))
		{
			return 0;
		}
	}

	ghInstance = hInstance;

	if (NULL == (hwnd = CreateWindow(WindowClassName, WindowTitle, 
		WS_CAPTION | WS_SYSMENU,			
		CW_USEDEFAULT, 0, SCREENWIDTH, SCREENHEIGHT, 
		NULL, NULL, hInstance, NULL)))
	{
		return 0;
	}
	if (!hwnd)
	{
		return 0;
	}

	if (FAILED(device.InitialDirect3D(hwnd)))
	{
		return 0;
	}

	pyramid.device = device;
	pyramid.AddRefD3D();
	
	cylinder.device = device;
	cylinder.AddRefD3D();
	
	plane.device = device;
	plane.AddRefD3D();
	if ((cylinder.InitialObject()) && (pyramid.InitialObject()) && (plane.InitialObject()))
	{
		if (FAILED(pyramid.device.shader.InitialShader(VertexDeclaration, D3D::SPHERA_FILE)))
		{
			return 0;
	    }
		if (FAILED(cylinder.device.shader.InitialShader(SkinningDeclaration, D3D::CYLINDER_FILE)))
		{
			return 0;
		}
		if (FAILED(plane.device.shader.InitialShader(VertexDeclaration, D3D::PLANE_FILE)))
		{
			return 0;
		}
		pyramid.device.shader.CreateShadow(D3D::SPHERA_SHADOW);
		cylinder.device.shader.CreateShadow(D3D::CYLINDER_SHADOW);

	}
	else 
	{
		return 0;
	}
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		 else
		{
			Render();
		}
	}

	return 0;
}

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
				device.camera.MoveCamera(deltaR, 0, 0);
				break;
		case VK_DOWN:
				device.camera.MoveCamera(-deltaR, 0, 0);
				break;
		case 'A':
				device.camera.MoveCamera(0, 0, deltaTetha);
				break;
		case 'D':
				device.camera.MoveCamera(0, 0, -deltaTetha);
				break;
		case 'W':
				device.camera.MoveCamera(0, -deltaPhi, 0);
				break;
		case 'S':
				device.camera.MoveCamera(0, deltaPhi, 0);
				break;
		case VK_SPACE:
			if (!device._wireframe())
			{
				device.Get3DDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
				device.ChangeWireframe(true);
			}
			else
			{ 
				device.Get3DDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				device.ChangeWireframe(false);
			}
			break;
		} 
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Render()
{
	if (NULL == device.GetDirect3D())
	{
		return;
	}
	device.Get3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 100, 130), 1.0f, 0);
	device.Get3DDevice()->BeginScene();
	device.light.SetLightForShader();
	D3DXMATRIX m = plane.CreateShadowMatrix(device.light.PointLightPosition);
    pyramid.SetShadowMatrix(m);
	cylinder.SetShadowMatrix(m);
	device.Get3DDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	plane.Draw(false);
	device.Get3DDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
	device.Get3DDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
    device.Get3DDevice()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    device.Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pyramid.Draw(true);  //shadow
	cylinder.Draw(true); //shadow
	
	device.Get3DDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
    device.Get3DDevice()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    device.Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	cylinder.Draw(false); //model
	pyramid.Draw(false);  //model
	device.Get3DDevice()->EndScene();
	device.Get3DDevice()->Present(NULL, NULL, NULL, NULL);
}

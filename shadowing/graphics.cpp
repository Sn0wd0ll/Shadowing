#include "graphics.h"

using namespace D3D;

Graphics::Graphics():pdirect3D(NULL), 
					 pdirect3DDevice(NULL),
					 pVertexBuffer(NULL),
					 pIndexBuffer(NULL),
					 wireframe(false)
{
	shader._device(this);
	matrix._device(this);
	light._device(this);
}
Graphics::~Graphics()
{
	if (NULL != pdirect3DDevice)
	{
		pdirect3DDevice->Release();
		pdirect3DDevice = NULL;
	}
	if (NULL != pdirect3D)
	{
		pdirect3D->Release();
		pdirect3D = NULL;
	}
	if (NULL != pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = NULL;
	}
	if (NULL != pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = NULL;
	}
}

HRESULT Graphics::InitialDirect3D(HWND hwnd)
 {
	if (NULL == (pdirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}
	D3DDISPLAYMODE Display;
	if (FAILED (pdirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))
	{
		return E_FAIL;
	}
	D3DPRESENT_PARAMETERS direct3DParametr;
	ZeroMemory(&direct3DParametr, sizeof(direct3DParametr));
	direct3DParametr.Windowed = TRUE;
	direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;
	direct3DParametr.BackBufferFormat = Display.Format;
	direct3DParametr.EnableAutoDepthStencil = TRUE;
	direct3DParametr.hDeviceWindow = hwnd;
	direct3DParametr.AutoDepthStencilFormat = D3DFMT_D24S8;

	if (FAILED (pdirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &direct3DParametr, &pdirect3DDevice)))
	{
		return E_FAIL;
	}
	//pdirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pdirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pdirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pdirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pdirect3DDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pdirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pdirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	return S_OK;
}

HRESULT Graphics::InitialVertexBuffer(int size, int CountVertex)
{
	if (FAILED(pdirect3DDevice->CreateVertexBuffer(CountVertex*size, 0, 0,
			D3DPOOL_MANAGED,&pVertexBuffer, NULL)))
	{
		return E_FAIL;
	}
	return S_OK;	
}

HRESULT Graphics::InitialIndexBuffer(int CountIndex)
{
	if (FAILED(pdirect3DDevice->CreateIndexBuffer(CountIndex*sizeof(CUSTOMINDEX), 0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED,&pIndexBuffer, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

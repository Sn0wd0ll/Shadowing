#include "graphics.h"
using namespace D3D;
Shader::Shader(): pVShader(NULL), pCode(NULL), pDeclaration(NULL), pVShadowDeclaration(NULL), pShadowShader(NULL)
{}

Shader::~Shader()
{
	if (NULL != pVShader)
	{
		pVShader->Release();
		pVShader = NULL;
	}
	if (NULL != pShadowShader)
	{
		pShadowShader->Release();
		pShadowShader = NULL;
	}
	if (NULL != pVDeclaration)
	{
		pVDeclaration->Release();
		pVDeclaration = NULL;
	}
	if (NULL != pVShadowDeclaration)
	{
		pVShadowDeclaration->Release();
		pVShadowDeclaration = NULL;
	}
}

HRESULT Shader::InitialShader(const D3DVERTEXELEMENT9 declaration[], const TCHAR * shader_name)
{
	D3DCAPS9 Caps;
	device->Get3DDevice()->GetDeviceCaps(&Caps);
	if (Caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		return E_FAIL;
	}
	
	if (D3D_OK != device->Get3DDevice()->CreateVertexDeclaration(declaration, &pVDeclaration))
	{
		return E_FAIL;
	}
	if (D3D_OK != D3DXAssembleShaderFromFile(shader_name, NULL, NULL, 0, &pCode, NULL))
	{	
		return E_FAIL;
	}
	
	if (D3D_OK != device->Get3DDevice()->CreateVertexShader(static_cast<DWORD*>(pCode->GetBufferPointer()), &pVShader))
	{	
		return E_FAIL;
	}
	if (NULL == pVShader)
	{	
		return E_FAIL;
	}	 
	pCode->Release();
	return S_OK;
}
HRESULT Shader::CreateShadow(const TCHAR * shader_name)
{

	if (D3D_OK != D3DXAssembleShaderFromFile(shader_name, NULL, NULL, 0, &pCode, NULL))
	{	
		return E_FAIL;
	}
	
	if (D3D_OK != device->Get3DDevice()->CreateVertexShader(static_cast<DWORD*>(pCode->GetBufferPointer()), &pShadowShader))
	{	
		return E_FAIL;
	}
	if (NULL == pShadowShader)
	{	
		return E_FAIL;
	}
	pCode->Release();
	return S_OK;
}
HRESULT Shader::SetShader(IDirect3DVertexShader9 *pShader)
{
	if (D3D_OK != device->Get3DDevice()->SetVertexDeclaration(pVDeclaration))
	{
		return E_FAIL;
	}
	if (D3D_OK != device->Get3DDevice()->SetVertexShader(pShader))
	{	
		return E_FAIL;
	}
	return S_OK;
}
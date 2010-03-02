#include "graphics.h"
#include "time.h"
using namespace D3D;
const float RATE            = D3DX_PI;
const float PERIOD			= 1.0f;
const float SPHERA_RATE		= 2.0f*D3DX_PI/PERIOD;
const float SphereRadius	= 1.41f;
const float SPEED			= 10.0f;
const float SHIFT_POS		= 1.5f;
Matrix::Matrix()
{}

Matrix::~Matrix()
{}
HRESULT Matrix::SetPlaneMatrix()
{
	ViewMatrix = device->camera.RotateCamera();
    const float M = 1.0f;
    const float F = 20.0f;
	assert(0 != (F-M));
    float a = F/(F - M);
    float b = -M * a;
	D3DXMATRIX ProjMatrix (M, 0,  0,  0,
					       0, M,  0,  0,
					       0, 0,  a,  b,
					       0, 0,  1,  0);
	D3DXMATRIX WorldMatrix =  ProjMatrix * ViewMatrix;
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SWMATRIX, static_cast<float*>(WorldMatrix), 4))
	{
		return E_FAIL;
	}
		if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(EYE, static_cast<float*>(device->camera.GetEye()), 1) )
	{
		
		return E_FAIL;
	}
	D3DXMATRIX ShiftPosition (1, 0,  0, 0,
					       0, 1,  0,  0,
					       0, 0,  1,  -SHIFT_POS,
					       0, 0,  0,  1);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SHIFT, static_cast<float*>(ShiftPosition), 4))
	{
		return E_FAIL;
	}
	return S_OK;
}
HRESULT Matrix::SetSkinningMatrix()
{
	ViewMatrix = device->camera.RotateCamera();
    const float M = 1.0f;
    const float F = 20.0f;
	assert(0 != (F-M));
    float a = F/(F - M);
    float b = -M * a;
	D3DXMATRIX ProjMatrix (M, 0,  0,  0,
					       0, M,  0,  0,
					       0, 0,  a,  b,
					       0, 0,  1,  0);
	D3DXMATRIX WorldMatrix =  ProjMatrix * ViewMatrix;
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SWMATRIX, static_cast<float*>(WorldMatrix), 4))
	{
		return E_FAIL;
	}
	float time = static_cast<float>(clock())/static_cast<float>(CLOCKS_PER_SEC);
    float rate = sin(RATE*time)/2;
		
	D3DXMATRIX RotateMatrix( 1,     0,          0,     0,
							 0,  cos(rate), sin(rate), 0,
                             0, -sin(rate), cos(rate), 0,
                             0,     0,          0,     1 );
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(BONE_1, RotateMatrix, 4))
	{
		return E_FAIL;
	}   
	
	D3DXMATRIX IDMatrix( 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1 );
    if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(BONE_2, IDMatrix,4))
    {
		return E_FAIL;
	}
	
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(EYE, static_cast<float*>(device->camera.GetEye()), 1) )
	{
		
		return E_FAIL;
	}
	D3DXMATRIX ShiftPosition (1, 0,  0, SHIFT_POS,
					       0, 1,  0,  0,
					       0, 0,  1,  0,
					       0, 0,  0,  1);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SHIFT, static_cast<float*>(ShiftPosition), 4))
	{
		return E_FAIL;
	}
	return S_OK;
	
}

HRESULT Matrix::SetMorphingMatrix()
{
	ViewMatrix = device->camera.RotateCamera();
	const float M = 1.0f;
    const float F = 20.0f;
    assert(0 != (F-M));
	float a = F/(F - M);
	float b = -M * a;
	D3DXMATRIX ProjMatrix (M, 0,  0,  0,
					       0, M,  0,  0,
					       0, 0,  a,  b,
					       0, 0,  1,  0);

	D3DXMATRIX WorldMatrix = ProjMatrix * ViewMatrix;
	D3DXMATRIX ShiftPosition (1, 0,  0, -SHIFT_POS,
					       0, 1,  0,  0,
					       0, 0,  1,  0,
					       0, 0,  0,  1);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(MATRIX, static_cast<float*>(WorldMatrix), 4))
	{
		return E_FAIL;
	}
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SHIFT, static_cast<float*>(ShiftPosition), 4))
	{
		return E_FAIL;
	}
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(EYE, static_cast<float*>(device->camera.GetEye()), 1) )
	{
		return E_FAIL;
	}
	float time = static_cast<float>(clock()*SPEED)/static_cast<float>(CLOCKS_PER_SEC);
    float t = (sin(D3DXToRadian(SPHERA_RATE*time)) + 1.0f)/2.0f; //0.5f;
	D3DXVECTOR4 v(t, 0.0f, 0.0f, 0.0f);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(TIME, v, 1))
	{
		return E_FAIL;
	}
	v.x = SphereRadius;
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(RADIUS, v, 1))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

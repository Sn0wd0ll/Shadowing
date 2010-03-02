#include "models.h"

const D3DXVECTOR3 SHADER_VAL_SHADOW_ATTENUATION  (1.3f, 0.1f, 0.14f);
bool Model::SetShadowMatrix(D3DXMATRIX m) 
{
	shadow_matrix = m; 
	if( D3D_OK != device.Get3DDevice()->SetVertexShaderConstantF(D3D::SHADOW_MATRIX,shadow_matrix, 4) )
	{
		return false;
	}
	if( D3D_OK != device.Get3DDevice()->SetVertexShaderConstantF(D3D::SHADOW_K,SHADER_VAL_SHADOW_ATTENUATION, 4) )
	{
		return false;
	}
	return true;
}
void Model::AddRefD3D()
{
	device.GetDirect3D()->AddRef();
	device.Get3DDevice()->AddRef(); 
}
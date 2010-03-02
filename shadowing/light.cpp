#include "graphics.h"
using namespace D3D;

const float INNER_CONE = cosf(D3DX_PI/20);
const float OUTER_CONE = cosf(D3DX_PI/12);
//const float OUTER_CONE = cosf(D3DX_PI/2);
enum
{
	DIRECT_LIGHT_DIRECTION = 12,
	DIRECT_LIGHT_COLOR     = 13,
	DIRECT_COEFFICIENT     = 14,
	AMBIENT_LIGHT_COLOR    = 15,
	POINT_LIGHT_COLOR      = 16,
	POINT_LIGHT_POSITION   = 17,
	ATTENUATION			   = 18,
	SPECULAR_LIGHT         = 19,
	GLARE                  = 20,
	SPOT_LIGHT_POSITION    = 22,
	SPOT_LIGHT_COLOR       = 23,
	SPOT_LIGHT_DIRECTION   = 24,
	ANGLE1				   = 25,
	ANGLE2                 = 26
};
Light::Light():
      DirLightDirection(0.0f, 100.0f, 100.0f)
	, DirLightColor(0.0f, 0.0f, 0.5f)
	, DifKoef(0.5f)
	, AmbientLightColor(0.6f, 0.6f, 0.6f)
	, PointLightPosition(0.0f, 0.0f, 3.4f)
	, PointLightColor(0.8f, 0.8f, 0.8f)
	, SpecularKoef(0.7f)
	, SpotLightDirection(3.2f,0,1.0f)//(1.0f, 1.0f, 0.5f)
	, SpotLightPosition(3.5,0,1)//(1.5f, 1.5f, -1.0f)
	, SpotLightColor(0.0f, 0.0f, 0.0f)
	, glare(25.0f, 25.0f, 25.0f)
	, attenuation(1.0f, 0.0f, 0.5f)
{
	D3DXVec3Normalize(&DirLightDirection, &DirLightDirection);
	D3DXVec3Normalize(&SpotLightDirection, &SpotLightDirection);
}

Light::~Light()
{}

bool Light::SetLightForShader()
{
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(DIRECT_LIGHT_DIRECTION, static_cast<float*>(DirLightDirection), 1) )
	{
		return false;
	}

	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(DIRECT_LIGHT_COLOR, static_cast<float*>(DirLightColor), 1) )
	{
		return false;
	}
	D3DXVECTOR4 v1(DifKoef,DifKoef,DifKoef,DifKoef);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(DIRECT_COEFFICIENT, v1, 1) )
	{
		return false;
	}
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(AMBIENT_LIGHT_COLOR, static_cast<float*>(AmbientLightColor), 1) )
	{
		return false;
	}
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(POINT_LIGHT_COLOR, static_cast<float*>(PointLightColor), 1) )
	{
		return false;
	}
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(POINT_LIGHT_POSITION, static_cast<float*>(PointLightPosition), 1) )
	{
		return false;
	}

	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(ATTENUATION, static_cast<float*>(attenuation), 1) )
	{
		return false;
	}
	D3DXVECTOR4 v2 (SpecularKoef,SpecularKoef,SpecularKoef,SpecularKoef);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SPECULAR_LIGHT, v2, 1) )
	{
		return false;
	}

	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(GLARE, static_cast<float*>(glare), 1) )
	{
		
		return false;
	}

	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SPOT_LIGHT_POSITION, static_cast<float*>(SpotLightPosition), 1) )
	{
		
		return false;
	}
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SPOT_LIGHT_COLOR, static_cast<float*>(SpotLightColor), 1) )
	{
		return false;
	}
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(SPOT_LIGHT_DIRECTION, static_cast<float*>(SpotLightDirection), 1) )
	{
		
		return false;
	}
	assert(0 != (INNER_CONE - OUTER_CONE));
	float f = 1/(INNER_CONE - OUTER_CONE);
	D3DXVECTOR4 angle1(f,f,f,f);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(ANGLE1,angle1, 1) )
	{
		return false;
	}
	f = OUTER_CONE/(INNER_CONE - OUTER_CONE);
	D3DXVECTOR4 angle2(f,f,f,f);
	if( D3D_OK != device->Get3DDevice()->SetVertexShaderConstantF(ANGLE2, angle2, 1) )
	{
		
		return false;
	}
	return true;
}
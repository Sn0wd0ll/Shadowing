#include "graphics.h"
using namespace D3D;
const float R_MIN = 1.74f;
const float R_MAX = 20.0f;
const float T_MIN = 0.01f;
const float T_MAX = D3DX_PI - 0.01f;
Camera::Camera():at(0, 0, 0), eye_spheric(5, D3DX_PI/2, -D3DX_PI/2), up(0, 0, 1)
{}

Camera::~Camera()
{}

void Camera::MoveCamera(float rMove, float thetaMove, float phiMove)
{
	eye_spheric.x +=rMove;
	if (R_MIN > eye_spheric.x)
	{
		eye_spheric.x = R_MIN;
	}
	if (R_MAX < eye_spheric.x)
	{
		eye_spheric.x = R_MAX;
	}
	eye_spheric.y +=thetaMove;
	if (T_MIN > eye_spheric.y)
	{
		eye_spheric.y = T_MIN;
	}
	if (T_MAX < eye_spheric.y)
	{
		eye_spheric.y = T_MAX;
	}
	eye_spheric.z +=phiMove;
}

D3DXVECTOR3 SphericToCartisian(D3DXVECTOR3 spheric)
{
	float r = spheric.x, theta = spheric.y, phi = spheric.z;
	D3DXVECTOR3 cartesian( r*sin(theta)*cos(phi), r*sin(theta)*sin(phi), r*cos(theta) );
	return cartesian;
}

D3DXVECTOR3 Camera::GetEye()
{
	return SphericToCartisian(eye_spheric);
}

D3DXMATRIX Camera::RotateCamera()
{
	D3DXVECTOR3 eye = SphericToCartisian(eye_spheric);
	
	D3DXVECTOR3 x;
	D3DXVECTOR3 y;
	D3DXVECTOR3 z;
	z = at - eye;
	D3DXVec3Normalize(&z, &z);
	D3DXVec3Cross(&x, &up, &z);
	D3DXVec3Normalize(&x, &x);

	D3DXVec3Cross(&y, &z, &x);

	return D3DXMATRIX     (x.x, x.y, x.z, -D3DXVec3Dot(&eye, &x),
						   y.x, y.y, y.z, -D3DXVec3Dot(&eye, &y),
						   z.x, z.y, z.z, -D3DXVec3Dot(&eye, &z),
						   0,   0,   0,   1);
	
}
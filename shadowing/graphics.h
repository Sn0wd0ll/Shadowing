#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include "assert.h"
#include <tchar.h>
#include <vector> 
#include "vertex.h"
namespace D3D
{
	class Graphics;

	enum  {TIME    = 1,
		  RADIUS   = 0,
		  SWMATRIX = 0,
		  MATRIX   = 2,
		  SHIFT    = 27,
		  BONE_1   = 4,
		  BONE_2   = 8,
		  EYE      = 21,
		  SHADOW_MATRIX = 31,
		  SHADOW_K = 35};
	class Matrix
	{
	public:
		Matrix();
		virtual ~Matrix();
		HRESULT SetSkinningMatrix();
		HRESULT SetMorphingMatrix();
		HRESULT SetPlaneMatrix();
		void _device(Graphics *device){this->device = device;}
	private:
		Graphics *device;
		D3DXMATRIX ViewMatrix;
		D3DXMATRIX ProjMatrix;
		D3DXMATRIX WorldMatrix;

		 
	};
	class Light
	{
	public:
		Light();
		virtual ~Light();
		void _device(Graphics *device){this->device = device;}
		bool SetLightForShader();
		D3DXVECTOR3 PointLightPosition;
	private:
		Graphics *device;
		D3DXVECTOR3 DirLightDirection;
		D3DXVECTOR3 DirLightColor;
		D3DXVECTOR3 AmbientLightColor;
		D3DXVECTOR3 PointLightColor;
		D3DXVECTOR3 SpotLightColor;
		D3DXVECTOR3 SpotLightDirection;
		D3DXVECTOR3 SpotLightPosition;
		D3DXVECTOR3 glare;
		D3DXVECTOR3 attenuation;
		float DifKoef, SpecularKoef;
	};
	class Shader 
	{
	public:
		Shader();
		virtual ~Shader();
		void _device(Graphics *device){this->device = device;}
		HRESULT InitialShader(const D3DVERTEXELEMENT9 declaration[], const TCHAR * shader_name);
		HRESULT SetShader(IDirect3DVertexShader9 *pShader); 
		HRESULT CreateShadow(const TCHAR * shader_name); 
		inline IDirect3DVertexShader9 *GetpVShader() {return pVShader;}
		inline IDirect3DVertexShader9 *GetpShadowShader() {return pShadowShader;}
		inline IDirect3DVertexDeclaration9* GetpVDeclaration() {return pVDeclaration;}
	private:
		Graphics *device;
		LPDIRECT3DVERTEXDECLARATION9	pDeclaration;
		IDirect3DVertexShader9			*pShadowShader;
		IDirect3DVertexShader9			*pVShader;
		ID3DXBuffer						*pCode;
		IDirect3DVertexDeclaration9		*pVDeclaration;
		IDirect3DVertexDeclaration9		*pVShadowDeclaration;
	};
	class Camera
	{
	private:
		D3DXVECTOR3		at;
		D3DXVECTOR3		eye_spheric; // r, theta, phi
		D3DXVECTOR3		up;
	public:
		Camera();
		virtual ~Camera();
		D3DXVECTOR3 GetEye();
		D3DXMATRIX RotateCamera();
		void MoveCamera(float xMove, float yMove, float zMove);
	};
	class Graphics
	{
	public:
		Graphics();
		virtual ~Graphics();
		Matrix matrix;
		Shader shader;
		Light light;
		Camera camera;
		HRESULT InitialDirect3D(HWND hwnd);
		HRESULT InitialVertexBuffer(int size, int CountVertex);
		HRESULT InitialIndexBuffer(int CountIndex);
		inline LPDIRECT3D9 GetDirect3D()       {return pdirect3D;}
		inline LPDIRECT3DDEVICE9 Get3DDevice() {return pdirect3DDevice;}
		inline LPDIRECT3DVERTEXBUFFER9 GetVB() {return pVertexBuffer;}
		inline LPDIRECT3DINDEXBUFFER9 GetIB()  {return pIndexBuffer;}
		bool _wireframe(){return wireframe;}
		void ChangeWireframe(bool value) {wireframe = value;}
	private:
		LPDIRECT3D9						pdirect3D;
		LPDIRECT3DDEVICE9				pdirect3DDevice;
		LPDIRECT3DVERTEXBUFFER9			pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9			pIndexBuffer;
		bool wireframe;
	};
	const int STENCIL_REF_VALUE = 0;
	const TCHAR CYLINDER_FILE[] = _T("cylinder_shader.vsh");
	const TCHAR SPHERA_FILE[] = _T("sphera_shader.vsh");
	const TCHAR CYLINDER_SHADOW[] = _T("cylinder_shadow.vsh");
	const TCHAR SPHERA_SHADOW[] = _T("sphera_shadow.vsh");
	const TCHAR PLANE_FILE[] = _T("plane_shader.vsh");
}

#include "models.h"

using namespace MyCylinder;
using namespace D3D;
Cylinder::Cylinder():tetha(D3DX_PI/3), phi(D3DX_PI/6)
{
};

bool Cylinder::InitialObject()
{
	SKINNINGVERTEX Vertex[VERTEX_COUNT];
	ZeroMemory(&Vertex, sizeof(Vertex));
	CUSTOMINDEX Index[INDEX_COUNT];
	ZeroMemory(&Index, sizeof(Index));
	float Radius = 0.9f;
	float Height = 2.1f;
	short vertex_count = 0;
	short Index_count = 0;
	for(short i = 0; i <= HEIGHT; i++)
	{
		for(short j = 0; j < BASE; j++)
		{
			Vertex[vertex_count].x = Radius*cosf(j*2*D3DX_PI/BASE);
			Vertex[vertex_count].y = Radius*sinf(j*2*D3DX_PI/BASE);
			Vertex[vertex_count].z = Height*static_cast<float>(i)/HEIGHT;
			Vertex[vertex_count].normal.x = Radius*cosf(j*2*D3DX_PI/BASE);
			Vertex[vertex_count].normal.y = Radius*sinf(j*2*D3DX_PI/BASE);
			Vertex[vertex_count].normal.z = 0;
			Vertex[vertex_count].normal.w = 0;
			float H = static_cast<float>(HEIGHT);
			float I = static_cast<float>(i);
			Vertex[vertex_count].w1=I/H;
			Vertex[vertex_count].w2=1-I/H;
			Vertex[vertex_count].color = RED;//D3DCOLOR_XRGB(rand()%256, rand()%256, rand()%256);;
			if (0 != i)
            {
                Index[Index_count++] =  vertex_count- BASE; 
                Index[Index_count++] = vertex_count;        
                if ((BASE - 1) == j)    // last 
                {
                    Index[Index_count++] = vertex_count - 2*BASE + 1; 
                    Index[Index_count++] = vertex_count - BASE + 1; 
                }
            }
           	vertex_count++;
		}
	}
	if (S_OK != device.InitialVertexBuffer(sizeof(SKINNINGVERTEX), VERTEX_COUNT))
	{
		return false;
	}
	
	void *pVB;
	if (FAILED(device.GetVB()->Lock(0, sizeof(Vertex[0])*VERTEX_COUNT, &pVB,0)))
	{
		return false;
	}
	memcpy(pVB, Vertex, sizeof(Vertex[0])*VERTEX_COUNT);
	device.GetVB()->Unlock();
	if (S_OK != device.InitialIndexBuffer(INDEX_COUNT))
	{
		return false;
	}
	void *pIB;
	if (FAILED(device.GetIB()->Lock(0, sizeof(Index[0])*INDEX_COUNT, &pIB,0)))
	{
		return false;
	}
	memcpy(pIB, Index, sizeof(Index[0])*INDEX_COUNT);
	device.GetIB()->Unlock();
	return true;
}

bool Cylinder::Draw(bool shadow)
{
	if (S_OK == device.matrix.SetSkinningMatrix())
	{

		if (D3D_OK == device.Get3DDevice()->SetStreamSource(0, device.GetVB(), 0, sizeof(SKINNINGVERTEX)))
		{
			if (shadow)
			{
				device.shader.SetShader(device.shader.GetpShadowShader());
			}
			else
			{
				device.shader.SetShader(device.shader.GetpVShader());
			}
			device.Get3DDevice()->SetIndices(device.GetIB());
			device.Get3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX_COUNT, 0, INDEX_COUNT-2);
			return true;
		}
	}
	return false;
}
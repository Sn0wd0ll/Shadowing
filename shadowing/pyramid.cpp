#include "models.h"
using namespace D3D;
using namespace MyPyramid;
Pyramid::Pyramid():tetha(D3DX_PI/3), phi(D3DX_PI/6)
{
}
CUSTOMVERTEX AddVertex(TesselateVertex Vertex, int i1, int i2, D3DXVECTOR4 &normal)
{
	DWORD color = RED;//D3DCOLOR_XRGB(rand()%256, rand()%256, rand()%256);
	
	return CUSTOMVERTEX ((Vertex[i1].x+Vertex[i2].x)/2,
						(Vertex[i1].y+Vertex[i2].y)/2,
						(Vertex[i1].z+Vertex[i2].z)/2,
						 normal,	
						 color);
}
D3DXVECTOR4 CalculateNormal(CUSTOMVERTEX vertex1, CUSTOMVERTEX vertex2,
													CUSTOMVERTEX vertex3)
{	
	D3DXVECTOR4 v1( vertex2.x-vertex1.x, vertex2.y-vertex1.y, vertex2.z-vertex1.z,0);
	D3DXVECTOR4 v2( vertex3.x-vertex1.x, vertex3.y-vertex1.y,vertex3.z-vertex1.z,0);
	D3DXVECTOR4 v3( 1,1,1,1);
	D3DXVECTOR4 normal;
	D3DXVec4Cross(&normal, &v2, &v1, &v3);
	D3DXVec4Normalize(&normal, &normal);
	return normal;
}
void Pyramid::Tessellate(TesselateVertex &Vertex, TesselateIndex &Index, int i1, int i2, int i3, int tesselate_level)
{
	if (tesselate_level < TesselateLevel)
	{
		D3DXVECTOR4 normal(CalculateNormal(Vertex[i1],Vertex[i2],Vertex[i3] ));
		int newIndex = Vertex.size()-1;
		Vertex.push_back(AddVertex(Vertex, i1, i3, normal));
		Vertex.push_back(AddVertex(Vertex, i1, i2, normal));
		Vertex.push_back(AddVertex(Vertex, i2, i3, normal));
		
		tesselate_level++;
		Tessellate(Vertex, Index, i1, newIndex + 2, newIndex + 1, tesselate_level);
 		Tessellate(Vertex, Index, i2, newIndex + 3, newIndex + 2, tesselate_level);
 		Tessellate(Vertex, Index, i3, newIndex + 1, newIndex + 3, tesselate_level);
 		Tessellate(Vertex, Index, newIndex + 1, newIndex + 2, newIndex + 3, tesselate_level);
	}
	else
	{
		Index.push_back(static_cast<CUSTOMINDEX>(i1));
		Index.push_back(static_cast<CUSTOMINDEX>(i2));
		Index.push_back(static_cast<CUSTOMINDEX>(i3));
	}
}

bool Pyramid::InitialObject()
{
	const float coord1 = 1.0f;
	const float coord2 = 1.41f;
	D3DXVECTOR4 vect(1,1,1,1);
	CUSTOMVERTEX Vertex[] = 
	{
		CUSTOMVERTEX( coord1,  -coord1,  0,  D3DXVECTOR4(coord1,  -coord1,  0, 1),        RED),	
		CUSTOMVERTEX(-coord1,  -coord1,  0,  D3DXVECTOR4(-coord1,  -coord1, 0, 1),         RED),	
		CUSTOMVERTEX(-coord1,   coord1,  0,  D3DXVECTOR4(-coord1,   coord1, 0, 1),         RED),	
		CUSTOMVERTEX( coord1,   coord1,  0,  D3DXVECTOR4(coord1,   coord1,  0, 1),         RED),		
		CUSTOMVERTEX(      0,        0,   coord2, D3DXVECTOR4(   0, 0, coord2, 1),   RED),		
		CUSTOMVERTEX(      0,        0,  -coord2, D3DXVECTOR4( 0, 0,  -coord2, 1),    RED)};
	CountVertex = sizeof(Vertex) / sizeof(Vertex[0] );
	CUSTOMINDEX Index[] = 
	{ 
		0, 4, 3,	3, 4, 2,
        2, 4, 1,	1, 4, 0,
		0, 3, 5,	3, 2, 5,
        2, 1, 5,	1, 0, 5
	};
	CountIndex = sizeof(Index) / sizeof(Index[0] );
	PyramidVertex.erase(PyramidVertex.begin(), PyramidVertex.end());
	PyramidIndex.erase(PyramidIndex.begin(), PyramidIndex.end());
	for (int i = 0; i<CountVertex; ++i)
	{
		PyramidVertex.push_back(Vertex[i]);
	}
	
	Tessellate (PyramidVertex, PyramidIndex, 0, 4, 3, 0);
	Tessellate (PyramidVertex, PyramidIndex, 3, 4, 2, 0);
	Tessellate (PyramidVertex, PyramidIndex, 2, 4, 1, 0);
	Tessellate (PyramidVertex, PyramidIndex, 1, 4, 0, 0);
	Tessellate (PyramidVertex, PyramidIndex, 0, 3, 5, 0);
	Tessellate (PyramidVertex, PyramidIndex, 3, 2, 5, 0);
	Tessellate (PyramidVertex, PyramidIndex, 2, 1, 5, 0);
	Tessellate (PyramidVertex, PyramidIndex, 1, 0, 5, 0);
	
	CountVertex = PyramidVertex.size();
	if (S_OK != device.InitialVertexBuffer(sizeof(CUSTOMVERTEX), CountVertex))
	{
		return false;
	}
	void *pVB;
	if (S_OK != device.GetVB()->Lock(0, sizeof(PyramidVertex[0])*CountVertex, &pVB,0))
	{
		return false;
	}

	memcpy(pVB, &PyramidVertex[0], sizeof(PyramidVertex[0])*CountVertex);
	device.GetVB()->Unlock();

	CountIndex = PyramidIndex.size();
	if (D3D_OK != device.InitialIndexBuffer(CountIndex))
	{
		return false;
	}
	void *pIB;
	if (D3D_OK != device.GetIB()->Lock(0, sizeof(PyramidIndex[0])*CountIndex, &pIB,0))
	{
		return false;
	}
	memcpy(pIB, &PyramidIndex[0], sizeof(PyramidIndex[0])*CountIndex);
	device.GetIB()->Unlock();
	return true;
}


bool Pyramid::Draw(bool shadow)
{
	if (S_OK == device.matrix.SetMorphingMatrix())
	{
		if (D3D_OK == device.Get3DDevice()->SetStreamSource(0, device.GetVB(), 0, sizeof(CUSTOMVERTEX)))
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
			device.Get3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, CountVertex, 0, CountIndex/3); 
			return true;
		}
	}
	return false;
}
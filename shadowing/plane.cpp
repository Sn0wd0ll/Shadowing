#include "models.h"
using namespace MyPlane;
const D3DXVECTOR3 normal(0, 0, 1);
const D3DXVECTOR3 position(0, 0, -1.3f);
Plane::Plane()
{
}
bool Plane::InitialObject()
{
	CUSTOMVERTEX Vertex[PLANE_VERTEX_COUNT];
	ZeroMemory(&Vertex, sizeof(Vertex));
	CUSTOMINDEX Index[PLANE_INDEX_COUNT];
	ZeroMemory(&Index, sizeof(Index));
    unsigned int vertex = 0; // current vertex
    unsigned int index = 0; // current index
	const float length = 10;
	const float width = 10;
	
	assert(( 0 != PLANE_STEPS));
    const float x_step = length/(2*PLANE_STEPS);
    const float y_step = width/(2*PLANE_STEPS);
    const int vertices_in_line = (2*PLANE_STEPS + 1);
	unsigned int cur_index = 0;
    
 
    for( int i = -PLANE_STEPS; i <= PLANE_STEPS; ++i )
    {
        for( int j = -PLANE_STEPS; j <= PLANE_STEPS; ++j )
        {
			Vertex[vertex].x = x_step*i;
			Vertex[vertex].y = y_step*j;
			Vertex[vertex].z = 0;
			Vertex[vertex].color = WHITE;
			Vertex[vertex].normal.x = normal.x;
			Vertex[vertex].normal.y = normal.y;
			Vertex[vertex].normal.z = normal.z;
			Vertex[vertex].normal.w = 0;
            if( i != -PLANE_STEPS && j != -PLANE_STEPS)
            {
            	Index[cur_index++] = vertex;
				Index[cur_index++] = vertex-1;
				Index[cur_index++] = vertex-1-vertices_in_line;

				Index[cur_index++] = vertex;
				Index[cur_index++] = vertex-1-vertices_in_line;
				Index[cur_index++] = vertex-vertices_in_line;
            }
            ++vertex;
        }
    }
	if (S_OK != device.InitialVertexBuffer(sizeof(CUSTOMVERTEX), PLANE_VERTEX_COUNT))
	{
		return false;
	}
	
	void *pVB;
	if (FAILED(device.GetVB()->Lock(0, sizeof(Vertex[0])*PLANE_VERTEX_COUNT, &pVB,0)))
	{
		return false;
	}
	memcpy(pVB, Vertex, sizeof(Vertex[0])*PLANE_VERTEX_COUNT);
	device.GetVB()->Unlock();
	if (S_OK != device.InitialIndexBuffer(PLANE_INDEX_COUNT))
	{
		return false;
	}
	void *pIB;
	if (FAILED(device.GetIB()->Lock(0, sizeof(Index[0])*PLANE_INDEX_COUNT, &pIB,0)))
	{
		return false;
	}
	memcpy(pIB, Index, sizeof(Index[0])*PLANE_INDEX_COUNT);
	device.GetIB()->Unlock();
	return true;

}

bool Plane::Draw(bool shadow)
{
	if (S_OK == device.matrix.SetPlaneMatrix())
	{
		if (D3D_OK == device.Get3DDevice()->SetStreamSource(0, device.GetVB(), 0, sizeof(CUSTOMVERTEX)))
		{
			device.shader.SetShader(device.shader.GetpVShader());
			device.Get3DDevice()->SetIndices(device.GetIB());
			device.Get3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, PLANE_VERTEX_COUNT, 0, PLANE_INDEX_COUNT/3); 
			return true;
		}
	}
	return false;
}
D3DXMATRIX Plane::CreateShadowMatrix(D3DXVECTOR3 light_position) 
{
    D3DXVECTOR3 const &n = normal;
    D3DXVECTOR3 const &L = light_position;
	float d = D3DXVec3Dot( &position, &normal );
    float L_n = D3DXVec3Dot(&L, &n);
 
    D3DXMATRIX A1( d, 0, 0, -d*L.x,
                   0, d, 0, -d*L.y,
                   0, 0, d, -d*L.z,
                   0, 0, 0,  0 );
 
    D3DXMATRIX A2( L_n,       0,       0, 0,
                         0, L_n,       0, 0,
                         0,       0, L_n, 0,
                         0,       0,       0, 0 );
 
    D3DXMATRIX A3( L.x*n.x, L.x*n.y, L.x*n.z, 0,
                   L.y*n.x, L.y*n.y, L.y*n.z, 0,
                   L.z*n.x, L.z*n.y, L.z*n.z, 0,
                   0, 0, 0, 0 );
 
    D3DXMATRIX A4( 0,   0,   0,  0,
                   0,   0,   0,  0,
                   0,   0,   0,  0,
                   n.x, n.y, n.z, -L_n );
 
    return A1 - A2 + A3 + A4;
}
#pragma once
#include "graphics.h"

class Model
{
public:
	Model(){};
	virtual ~Model(){};
	D3D::Graphics device;
	bool SetShadowMatrix(D3DXMATRIX m);
	void AddRefD3D();
protected:
	D3DXMATRIX shadow_matrix;
	virtual bool Draw(bool shadow) = 0;
	virtual bool InitialObject() = 0;
	int CountVertex;
	int CountIndex;
};

namespace MyCylinder
{
const int HEIGHT = 130;
const int BASE = 100;
const int VERTEX_COUNT = (HEIGHT+1)*(BASE);
const int INDEX_COUNT = 2*HEIGHT*(BASE+1);

class Cylinder : public Model
{
public:
	Cylinder();
	virtual ~Cylinder(){};
	virtual bool Draw(bool shadow);
	virtual bool InitialObject();
private:
	float tetha, phi;
};
}
namespace MyPyramid
{
const unsigned TesselateLevel = 4;
typedef std::vector<CUSTOMVERTEX> TesselateVertex;
typedef std::vector<CUSTOMINDEX> TesselateIndex;

class Pyramid : public Model
{
public:
	Pyramid();
	virtual ~Pyramid() {};
	virtual bool Draw(bool shadow);
	virtual bool InitialObject();
	D3DXMATRIX CreateShadowMatrix(D3DXVECTOR3 light_position);
private:
	float tetha, phi;
	TesselateVertex PyramidVertex;
	TesselateIndex PyramidIndex;
	void Tessellate(TesselateVertex &Vertex, TesselateIndex &Index, int i1, int i2, int i3, int tesselate_level);
};
}
namespace MyPlane
{
const int PLANE_STEPS = 50;
const int PLANE_VERTEX_COUNT = (2*PLANE_STEPS + 1)*(2*PLANE_STEPS + 1);
const int PLANE_INDEX_COUNT = 2*3*(2*PLANE_STEPS)*(2*PLANE_STEPS);

class Plane : public Model
{
public:
	Plane();
	~Plane(){};
	virtual bool Draw(bool shadow);
	virtual bool InitialObject();
	D3DXMATRIX CreateShadowMatrix(D3DXVECTOR3 light_position); 
};
}
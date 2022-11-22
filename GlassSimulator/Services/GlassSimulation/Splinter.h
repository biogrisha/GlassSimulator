#pragma once
#include <vector>
#include <Services/GraphicStructs/GraphicStructs.h>
#include <Services/3DAuxiliaryFunctions/PlaneTriangulation/PlaneTriangulator.h>
struct Splinter
{
	std::vector<Triangle> m_triangles;
};

class Splinters
{
public:
	Splinters(const std::vector<Vertex>& points);
	std::vector<Splinter>& GetSplinters();
	std::vector<Vertex>& GetPoints();
private:
	std::vector<Vertex> m_points;
	std::vector<Splinter> m_splinters;
};
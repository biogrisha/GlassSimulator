#include "Splinter.h"

Splinters::Splinters(const std::vector<Vertex>& points)
{
	m_points = points;

	PlaneTriangulator triangulator;
	std::vector<Triangle> triangles;

	triangulator.ApplyTriangulation(m_points, triangles);

	auto old_count = m_points.size();
	m_points.resize(2 * old_count);
	std::copy_n(m_points.begin(), old_count, m_points.begin() + old_count);

	int pointsHalfSize = m_points.size() / 2;
	for (int i = pointsHalfSize; i < m_points.size(); i++)
	{
		m_points[i].pos.z += 0.1;
	}

	for (int i = 0; i < triangles.size(); i++)
	{
		Splinter splint;
		Triangle trig1;
		trig1.a = triangles[i].a + pointsHalfSize;
		trig1.b = triangles[i].b + pointsHalfSize;
		trig1.c = triangles[i].c + pointsHalfSize;
		splint.m_triangles.push_back(triangles[i]);
		splint.m_triangles.push_back(trig1);

		int* ind1 = &trig1.a;
		int* ind2 = &triangles[i].a;
		
		for (int i = 0; i < 2; i++)
		{
		
			Triangle trig2;
			trig2.a = *ind1;
			trig2.b = *ind2;
			trig2.c = *(ind2+1);
		
			splint.m_triangles.push_back(trig2);
		
			trig2.a = *(ind2 + 1);
			trig2.b = *(ind1 + 1);
			trig2.c = *ind1;
			
			splint.m_triangles.push_back(trig2);
		
			ind1++;
			ind2++;
		
		
		}
		
		Triangle trig2;
		trig2.a = *ind1;
		trig2.b = *ind2;
		trig2.c = triangles[i].a;
		
		splint.m_triangles.push_back(trig2);
		
		trig2.a = triangles[i].a;
		trig2.b = trig1.a;
		trig2.c = *ind1;
		
		splint.m_triangles.push_back(trig2);
		
		
		m_splinters.push_back(std::move(splint));
	}



}

std::vector<Splinter>& Splinters::GetSplinters()
{
	return m_splinters;
}

std::vector<Vertex>& Splinters::GetPoints()
{
	return m_points;
}

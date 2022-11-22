#pragma once
#include  <vector>
#include <Services/GraphicStructs/GraphicStructs.h>
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <limits>



class PlaneTriangulator
{
public:
	/**
	 * Applies triangulation to the set of points on the XY plane.
	 * 
	 * CHANGES THE ORDER OF POINTS IN THE ARRAY!!
	 * @param vertices Points on the basis of which the triangulation will be performed.
	 * @param[out] triangles Resulting triangles.
	 */
	void ApplyTriangulation(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles);

private:

	bool IsVertOnTheRight(const glm::vec4& pointFrom, const glm::vec4& pointTo, const glm::vec4& pointToCheck);
	bool IsLineIntersectedByChain(std::vector<Vertex>& vertices, const glm::vec4& pointFrom, const glm::vec4& pointTo, int from, int to, int chainFrom, int chainTo);
	bool IsLineIntersectedByLines(std::vector<Triangle>& lines, std::vector<Vertex>& vertices, const glm::vec4& pointFrom, const glm::vec4& pointTo, int from, int to);
	bool IsLineIntersected(const glm::vec4& pointFrom1, const glm::vec4& pointTo1, const glm::vec4& pointFrom2, const glm::vec4& pointTo2);
	int FindFarthestEl(std::vector<Vertex>& vertices, const glm::vec4& point, int countToCheck);
};
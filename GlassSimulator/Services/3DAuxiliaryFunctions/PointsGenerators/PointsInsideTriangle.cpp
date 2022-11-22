#include "PointsInsideTriangle.h"

void PointsInsideTriangle::GeneratePoints(std::vector<Vertex>& pointsAppend, const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, int count)
{
	for (int i = 0; i < count; i++)
	{
		Vertex vert;
		vert.pos.x = (float(rand() % 100)) / 100;
		vert.pos.y = (float(rand() % 100)) / 100;
		bool isInUpperTriangle = GraphicHelpers::IsVertOnTheRight(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), vert.pos,0.001);

		if (isInUpperTriangle)
		{
			vert.pos.x = 1 - vert.pos.x;
			vert.pos.y = 1 - vert.pos.y;
		}
		glm::vec4 ab = b - a;
		glm::vec4 ac = c - a;

		auto vecTemp = vert.pos;
		vert.pos.x = (vecTemp.x* ab.x + vecTemp.y*ac.x)+a.x;
		vert.pos.y = (vecTemp.x* ab.y + vecTemp.y*ac.y)+a.y;

		pointsAppend.push_back(std::move(vert));
	}

	
}

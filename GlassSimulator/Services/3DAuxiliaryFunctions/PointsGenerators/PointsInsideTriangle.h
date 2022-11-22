#pragma once
#include <vector>
#include <Services/GraphicStructs/GraphicStructs.h>
#include <Services/3DAuxiliaryFunctions/Helpers/GraphicHelpers.h>
class PointsInsideTriangle
{
public:
	void GeneratePoints(std::vector<Vertex>& pointsAppend,const glm::vec4& a,const glm::vec4& b,const glm::vec4& c,int count);
};
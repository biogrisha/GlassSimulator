#pragma once
#include <Services/GraphicStructs/GraphicStructs.h>

namespace GraphicHelpers
{
	bool IsVertOnTheRight(const glm::vec4& pointFrom, const glm::vec4& pointTo, const glm::vec4& pointToCheck, float error);

}
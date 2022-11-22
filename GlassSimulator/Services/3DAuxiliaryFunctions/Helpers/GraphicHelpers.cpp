#include "GraphicHelpers.h"

bool GraphicHelpers::IsVertOnTheRight(const glm::vec4& pointFrom, const glm::vec4& pointTo, const glm::vec4& pointToCheck,float error)
{
    bool res = false;
    glm::vec4 ab = pointTo - pointFrom;
    glm::vec4 ac = pointToCheck - pointFrom;
    float xTransf = (ab.y * ac.x) - (ab.x * ac.y) + error;
    if (xTransf >= 0)
        res = true;
    else
    {
        res = false;
    }
    return res;
}

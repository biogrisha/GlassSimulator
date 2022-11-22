#include "PlaneTriangulator.h"

void PlaneTriangulator::ApplyTriangulation(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles)
{

    
    //Find center

    glm::vec4 center;
    for (auto& vert : vertices)
    {
        center += vert.pos;
    }
    center /= vertices.size();

    //Sort by dist
    auto sortLamb = [&center](const Vertex& vert1, const Vertex& vert2) -> bool
    {
        auto distVec1 = vert1.pos - center;
        auto distVec2 = vert2.pos - center;
        float v1Dist = pow(distVec1.x, 2) + pow(distVec1.y, 2);
        float v2Dist = pow(distVec2.x, 2) + pow(distVec2.y, 2);
        return v1Dist > v2Dist;
    };

    sort(vertices.begin(), vertices.end(), sortLamb);

    std::vector<int>chainOffsets;
    chainOffsets.push_back(0);
    int UsedElements = 0;

    //Create closed chains


    //Get last point which is a first point in the chain

    bool firstLoop = true;
    while (vertices.size() - UsedElements > 2)
    {
        if (firstLoop)
        {
            std::iter_swap(vertices.begin(), vertices.end() - 1 - UsedElements);
            firstLoop = false;
        }
        else
        {
            std::iter_swap(vertices.begin() + FindFarthestEl(vertices, center, vertices.size() - UsedElements), vertices.end() - 1 - UsedElements);

        }
        Vertex& firstChainEl = vertices[vertices.size() - UsedElements - 1];

        bool isChainClosed = false;
        for (int i = 0; i < vertices.size() - UsedElements; i++)
        {
            //Get point1 which is in the tail
            Vertex& el1 = vertices[vertices.size() - i - UsedElements - 1];

            //Check if point2 is good
            bool isGoodPoint = true;

            //If it is third or bigger then we check if chain is closed
            std::vector<int> nextPoints;
            if (i > 1)
            {
                for (int k = 0; k < vertices.size() - UsedElements - 1; k++)
                {
                    if (k == vertices.size() - i - UsedElements - 1)
                    {
                        continue;
                    }
                    isGoodPoint &= IsVertOnTheRight(el1.pos, firstChainEl.pos, vertices[k].pos);

                }
                if (isGoodPoint)
                {
                    nextPoints.push_back(vertices.size() - UsedElements - 1);
                }
            }
            for (int j = 0; j < vertices.size() - i - UsedElements - 1; j++)
            {
                isGoodPoint = true;
                Vertex& el2 = vertices[j];
                for (int k = 0; k < vertices.size() - UsedElements && isGoodPoint; k++)
                {
                    if (k == j || k== vertices.size() - i - UsedElements - 1)
                    {
                        continue;
                    }
                    isGoodPoint &= IsVertOnTheRight(el1.pos, el2.pos, vertices[k].pos);

                }
                if (isGoodPoint)
                {
                    nextPoints.push_back(j);
                }
            }

            if (nextPoints.size() == 1)
            {
                if (nextPoints.back() == vertices.size() - UsedElements - 1)
                {
                    UsedElements += i + 1;
                    chainOffsets.push_back(UsedElements);
                    break;
                }
                else
                {
                    std::iter_swap(vertices.begin() + nextPoints.back(), vertices.end() - 2 - UsedElements - i);
                }
            }
            else
            {
                float length = std::numeric_limits<float>::max();
                int nextInd = 0;
                for (int k = 0; k < nextPoints.size(); k++)
                {
                    auto distVec = el1.pos - vertices[nextPoints[k]].pos;
                    float dist = pow(distVec.x, 2) + pow(distVec.y, 2) + pow(distVec.z, 2);
                    if (length > dist)
                    {
                        length = dist;
                        nextInd = nextPoints[k];
                    }
                }
                if (nextInd == vertices.size() - UsedElements - 1)
                {
                    UsedElements += i + 1;
                    chainOffsets.push_back(UsedElements);
                    break;
                }
                else
                {
                    std::iter_swap(vertices.begin() + nextInd, vertices.end() - 2 - UsedElements - i);
                }
            }


        }
    }

    if (chainOffsets.back() != vertices.size())
        chainOffsets.push_back(vertices.size());

    std::reverse(vertices.begin(), vertices.end());

    //find inner joints of vertices
    //find closest points in inner ring to points in outter ring


    std::vector<Triangle> lines;

    int linesToCheckFrom = 0;

    int linesFrom = 0;
    int linesTo = 0;
    for (int i = 0; i < chainOffsets.size() - 3; i++)
    {

        for (int j = chainOffsets[i]; j < chainOffsets[i + 1]; j++)//outter chain
        {
            ModInt ind(chainOffsets[i + 1], chainOffsets[i + 2], chainOffsets[i + 1]);
            ModInt indPost(chainOffsets[i + 1], chainOffsets[i + 2], chainOffsets[i + 1]);
            ModInt indPrev(chainOffsets[i + 1], chainOffsets[i + 2], chainOffsets[i + 1]);
            bool isFirstSet = false;
            bool isLastSet = false;
            int firstInd = 0;
            for (int k = chainOffsets[i + 1]; k < chainOffsets[i + 2]; k++)//outter chain
            {

                bool cond = false;
                indPrev = ind - 1;
                indPost = ind + 1;
                cond = IsLineIntersectedByChain(vertices, vertices[j].pos, vertices[k].pos, indPost, indPrev, chainOffsets[i + 1], chainOffsets[i + 2]);
                cond |= IsLineIntersectedByLines(lines, vertices, vertices[j].pos, vertices[k].pos, linesToCheckFrom, lines.size());
                if (!cond)
                {
                    Triangle trig;
                    trig.a = j;
                    trig.b = k;
                    if (!isFirstSet)
                    {
                        trig.c = 1;//1 - means the first element
                        if (isLastSet && lines[firstInd].c != 2)
                            lines[firstInd].c = -1;
                        firstInd = lines.size();
                        isFirstSet = true;
                    }
                    if (k == chainOffsets[i + 2] - 1 && !isLastSet)
                    {
                        trig.c = 2;
                    }
                    lines.push_back(trig);
                }
                else if (isFirstSet && !isLastSet)
                {
                    lines[lines.size() - 1].c = 2;// 2 - means the last element
                    isLastSet = true;
                    isFirstSet = false;
                }
                ++ind;
            }


            //infer triangles
            linesTo = lines.size();
            ModInt outerChainInd(chainOffsets[i], chainOffsets[i + 1], lines[linesFrom].a);
            ModInt innerChainInd(chainOffsets[i + 1], chainOffsets[i + 2], chainOffsets[i + 1]);
            int end = 0;
            for (int w = linesFrom; w < linesTo; w++)
            {
                if (lines[w].c == 2)
                {
                    end = lines[w].b;
                }
                if (lines[w].c == 1)
                {

                    innerChainInd = lines[w].b;
                }
            }

            {
                Triangle triangle;
                triangle.a = outerChainInd;
                triangle.b = (outerChainInd + 1);
                triangle.c = end;

                triangles.push_back(triangle);
            }
            if (linesTo - linesFrom > 1)
            {
                for (; innerChainInd != end; innerChainInd++)
                {
                    Triangle triangle;
                    triangle.a = outerChainInd;
                    triangle.b = innerChainInd;
                    triangle.c = (innerChainInd + 1);

                    triangles.push_back(triangle);
                }
            }
            linesFrom = linesTo;
        }
        linesToCheckFrom = lines.size();

    }

    int firstPointOfLastChain = chainOffsets[chainOffsets.size() - 3];
    ModInt lastChainInd(chainOffsets[chainOffsets.size() - 3], chainOffsets[chainOffsets.size() - 2], chainOffsets[chainOffsets.size() - 3]+1);
    int lastChainLastInd =  ModInt(chainOffsets[chainOffsets.size() - 3], chainOffsets[chainOffsets.size() - 2], chainOffsets[chainOffsets.size() - 3]-1);
    
    for (; (int)lastChainInd != lastChainLastInd; lastChainInd++)
    {
        Triangle trig;
        trig.a = firstPointOfLastChain;
        trig.b = (int)lastChainInd;
        trig.c = (int)(lastChainInd + 1);
        triangles.push_back(trig);

    }
}

bool PlaneTriangulator::IsVertOnTheRight(const glm::vec4& pointFrom, const glm::vec4& pointTo, const glm::vec4& pointToCheck)
{
    bool res = false;
    glm::vec4 ab = pointTo - pointFrom;
    glm::vec4 ac = pointToCheck - pointFrom;
    float xTransf = (ab.y * ac.x) - (ab.x * ac.y)+0.0001;
    if (xTransf >= 0)
        res = true;
    else
    {
        res = false;
    }
    return res;
}

bool PlaneTriangulator::IsLineIntersectedByChain(std::vector<Vertex>& vertices, const glm::vec4& pointFrom, const glm::vec4& pointTo, int from, int to, int chainFrom, int chainTo)
{
    bool res = false;
    ModInt i(chainFrom, chainTo, from);
    for (; (int)i != to && !res; i++)
    {
        int indPlusOne = i + 1;
        if (indPlusOne == chainTo)
        {
            indPlusOne = chainFrom;
        }

        res |= IsLineIntersected(pointFrom, pointTo, vertices[i].pos, vertices[indPlusOne].pos);
    }

    return res;
}

bool PlaneTriangulator::IsLineIntersectedByLines(std::vector<Triangle>& lines, std::vector<Vertex>& vertices, const glm::vec4& pointFrom, const glm::vec4& pointTo, int from, int to)
{
    bool res = false;
    for (int i = from; i < to && !res; i++)
    {
        if (pointTo == vertices[lines[i].b].pos)
            continue;
        if (pointFrom == vertices[lines[i].a].pos)
            continue;
        res |= IsLineIntersected(pointFrom, pointTo, vertices[lines[i].a].pos, vertices[lines[i].b].pos);
    }

    return res;
}

bool PlaneTriangulator::IsLineIntersected(const glm::vec4& pointFrom1, const glm::vec4& pointTo1, const glm::vec4& pointFrom2, const glm::vec4& pointTo2)
{
    float y1 = pointFrom1.y;
    float y2 = pointFrom2.y;
    float x1 = pointFrom1.x;
    float x2 = pointFrom2.x;

    float dx1 = pointFrom1.x - pointTo1.x;
    float dy1 = pointFrom1.y - pointTo1.y;

    float dx2 = pointFrom2.x - pointTo2.x;
    float dy2 = pointFrom2.y - pointTo2.y;
    bool res = false;

    float xMax1 = pointFrom1.x;
    float xMin1 = pointTo1.x;


    if (dx1 < 0)
    {
        xMin1 = pointFrom1.x;
        xMax1 = pointTo1.x;

    }

    float xMax2 = pointFrom2.x;
    float xMin2 = pointTo2.x;


    if (dx2 < 0)
    {
        xMin2 = pointFrom2.x;
        xMax2 = pointTo2.x;

    }


    float yMax1 = pointFrom1.y;
    float yMin1 = pointTo1.y;


    if (dy1 < 0)
    {
        yMin1 = pointFrom1.y;
        yMax1 = pointTo1.y;

    }

    float yMax2 = pointFrom2.y;
    float yMin2 = pointTo2.y;


    if (dy2 < 0)
    {
        yMin2 = pointFrom2.y;
        yMax2 = pointTo2.y;

    }

    if ((dx1 == 0 && dy2 == 0))
    {
        bool cond1 = xMax1< xMax2&& xMax1 >xMin2;
        bool cond2 = yMax2< yMax1&& yMax2 >yMin1;
        res = cond1 && cond2;
    }
    else if ((dy1 == 0 && dx2 == 0))
    {
        bool cond1 = xMax2< xMax1&& xMax2 >xMin1;
        bool cond2 = yMax1< yMax2&& yMax1 >yMin2;
        res = cond1 && cond2;
    }
    else if (dx1 != 0 && dx2 != 0)
    {

        float xIntersect = -(y1 - y2 - (dy1 / dx1) * x1 + (dy2 / dx2) * x2) / ((dy1 / dx1) - (dy2 / dx2));
        res = (xIntersect > xMin1 && xIntersect < xMax1) && (xIntersect > xMin2 && xIntersect < xMax2);
    }
    else if (dy1 != 0 && dy2 != 0)
    {
        float yIntersect = -(x1 - x2 - (dx1 / dy1) * y1 + (dx2 / dy2) * y2) / ((dx1 / dy1) - (dx2 / dy2));
        res = (yIntersect > yMin1 && yIntersect < yMax1) && (yIntersect > yMin2 && yIntersect < yMax2);

    }
    else
    {
        res = false;
    }
    return res;
}

int PlaneTriangulator::FindFarthestEl(std::vector<Vertex>& vertices, const glm::vec4& point, int countToCheck)
{
    float squaredDist = 0;
    int ind = 0;
    for (int i = 0; i < countToCheck; i++)
    {
        auto distVec = vertices[i].pos - point;
        float dist = pow(distVec.x, 2) + pow(distVec.y, 2);
        if (dist > squaredDist)
        {
            ind = i;
            squaredDist = dist;
        }
    }

    return ind;
}

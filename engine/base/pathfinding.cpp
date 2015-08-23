#include "pathfinding.h"

bool areValidCoordinates(int x, int y, int w, int h)
{
    return (x >= 0 && y >= 0 && x < w && y < h);
}

int HFunc(int xStart, int yStart, int xDest, int yDest)
{
    return std::abs(xStart - xDest) + std::abs(yStart - yDest);
}
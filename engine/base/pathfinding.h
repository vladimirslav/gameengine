#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include <vector>

struct TileCoords
{
    int x;
    int y;
    bool operator==(const TileCoords& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct TileInfo
{
    TileCoords comeFrom;
    bool isStartingTile;
    bool isVisited;
    int G;
    int H;
};

using tile_list = std::vector<TileCoords>;

//template <typename T> using vacantCheckFunction = bool(*)(const T&);
//template<typename T> ;
// template <typename T> using valueFunc = int (*)valueFunc(int xStart, int yStart, int xDest, int yDest);

bool areValidCoordinates(int x, int y, int w, int h);
int HFunc(int xStart, int yStart, int xDest, int yDest);

template <typename T> tile_list Astar(T*** field,
                                      size_t w,
                                      size_t h, 
                                      bool(*checkFunc)(const T*),
                                      int xStart,
                                      int yStart,
                                      int xDest,
                                      int yDest,
                                      bool include_destination,
                                      int unit_w = 1)
{
    if (areValidCoordinates(xStart, yStart, w, h) == false)
    {
        return {}; //empty list
    }

    tile_list closed_set;
    tile_list open_set;

    std::vector<std::vector<TileInfo>> evaluatedCoords(w); // array of array

    for (size_t x = 0; x < w; x++)
    {
        evaluatedCoords[x] = std::vector<TileInfo>(h);
        for (size_t y = 0; y < h; y++)
        {
            evaluatedCoords[x][y].isVisited = false;
            evaluatedCoords[x][y].G = 0;
            evaluatedCoords[x][y].H = HFunc(x, y, xDest, yDest);
        }
    }

    bool destinationReached = false;
    // add starting tiles to open list
    for (int i = xStart; i < xStart + unit_w; i++)
    {

        evaluatedCoords[i][yStart].G = 0;
        evaluatedCoords[i][yStart].isStartingTile = true;
        evaluatedCoords[i][yStart].isVisited = true;

        if (areValidCoordinates(i, yStart, w, h))
        {
            open_set.push_back({ i, yStart });
        }
    }

    while (open_set.empty() == false && destinationReached == false)
    {
        TileCoords currentTile;
        int minF = w * h;
        size_t tileNum;
        // select lowest F
        for (size_t i = 0; i < open_set.size(); i++)
        {
            int F = evaluatedCoords[xStart][yStart].G + evaluatedCoords[xStart][yStart].H;
            if (F < minF)
            {
                tileNum = i;
                currentTile = open_set[tileNum];
                minF = F;
            }
        }
        TileCoords adjacentCoordinates[] = { { currentTile.x - 1, currentTile.y }, 
                                                { currentTile.x + 1, currentTile.y },
                                                { currentTile.x, currentTile.y - 1 },
                                                { currentTile.x, currentTile.y + 1 } };

        for (int i = 0; i < sizeof(adjacentCoordinates) / sizeof(*adjacentCoordinates); i++)
        {
            if (areValidCoordinates(adjacentCoordinates[i].x, adjacentCoordinates[i].y, w, h))
            {
                if (std::find(closed_set.begin(), closed_set.end(), adjacentCoordinates[i]) == closed_set.end())
                {
                    if (std::find(open_set.begin(), open_set.end(), adjacentCoordinates[i]) == open_set.end())
                    {
                        if (adjacentCoordinates[i].x == xDest && adjacentCoordinates[i].y == yDest)
                        {
                            evaluatedCoords[xDest][yDest].comeFrom.x = currentTile.x;
                            evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].comeFrom.y = currentTile.y;
                            destinationReached = true;
                            break;
                        }

                        bool validTiles = true;
                        for (int newX = adjacentCoordinates[i].x; newX < adjacentCoordinates[i].x + unit_w; newX++)
                        {
                            if (areValidCoordinates(newX, adjacentCoordinates[i].y, w, h) == false)
                            {
                                validTiles = false;
                                break;
                            }

                            if (checkFunc(field[newX][adjacentCoordinates[i].y]) == false)
                            {
                                validTiles = false;
                                break;
                            }
                        }

                        if (validTiles) // if we have an unocupied field
                        {
                            evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].comeFrom.x = currentTile.x;
                            evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].comeFrom.y = currentTile.y;
                            evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].G = evaluatedCoords[currentTile.x][currentTile.y].G + 1;
                            open_set.push_back(adjacentCoordinates[i]);
                        }
                    }
                    else
                    {
                        if (evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].G > evaluatedCoords[currentTile.x][currentTile.y].G + 1)
                        {
                            evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].comeFrom.x = currentTile.x;
                            evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].comeFrom.y = currentTile.y;
                            evaluatedCoords[adjacentCoordinates[i].x][adjacentCoordinates[i].y].G = evaluatedCoords[currentTile.x][currentTile.y].G + 1;
                        }
                    }
                    /*
                    if (currentTile.x == xDest && currentTile.y == yDest)
                    {
                        destinationReached = true; //found our objective
                    }
                    */
                }
            }
        }

        closed_set.push_back(currentTile);
        open_set.erase(open_set.begin() + tileNum);
    }

    if (destinationReached)
    {
        tile_list path;
        if (include_destination)
        {
            path.push_back({ xDest, yDest });
        }

        TileCoords currentTile = evaluatedCoords[xDest][yDest].comeFrom;

        while (currentTile.x != xStart || currentTile.y != yStart)
        {
            path.push_back(currentTile);
            currentTile = evaluatedCoords[currentTile.x][currentTile.y].comeFrom;
        }

        return path;
    }
    else
    {
        return {};
    }
}


#endif

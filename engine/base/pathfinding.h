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
                                      bool(*checkFunc)(int x, int y),
                                      int xStart,
                                      int yStart,
                                      int xDest,
                                      int yDest,
                                      bool include_destination,
                                      int unit_w)
{
    if (areValidCoordinates(xStart, yStart, w, h) == false)
    {
        return {}; //empty list
    }

    // check if unit can actually fit in destination
    if (include_destination)
    {
        for (int newX = xDest; newX < xDest + unit_w; newX++)
        {
            if (areValidCoordinates(newX, yDest, w, h) == false)
            {
                return{}; // empty list, one of the destination tiles is out of bounds
            }

            if (checkFunc(newX, yDest) == false)
            {
                return{}; // empty list, one of the destination tiles is occupied
            }
        }
    }

    tile_list closed_set;
    tile_list open_set;

    std::vector<std::vector<TileInfo>> evaluatedCoords(w); // array of array

    // initialize values, calculate initial predictions:
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
    // add starting tile to open list
    evaluatedCoords[xStart][yStart].G = 0;
    evaluatedCoords[xStart][yStart].isStartingTile = true;
    evaluatedCoords[xStart][yStart].isVisited = true;

    if (areValidCoordinates(xStart, yStart, w, h))
    {
        open_set.push_back({ xStart, yStart });
    }

    // while we have not reached destination
    // and there are tiles that can still be evaluated
    while (open_set.empty() == false && destinationReached == false)
    {
        TileCoords currentTile;
        int minF = w * h; // minimum cost that is required to reach destination
        size_t tileNum;   // tile index number
        // select assumed lowest-cost path
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

        // make an array of adjacent coordinates
        TileCoords adjacentCoordinates[] = { { currentTile.x - 1, currentTile.y }, 
                                                { currentTile.x + 1, currentTile.y },
                                                { currentTile.x, currentTile.y - 1 },
                                                { currentTile.x, currentTile.y + 1 } };

        // ... then go through it and check the new possible tiles
        for (int i = 0; i < sizeof(adjacentCoordinates) / sizeof(*adjacentCoordinates); i++)
        {
            if (areValidCoordinates(adjacentCoordinates[i].x, adjacentCoordinates[i].y, w, h))
            {
                if (std::find(closed_set.begin(), closed_set.end(), adjacentCoordinates[i]) == closed_set.end())
                {
                    int adjX = adjacentCoordinates[i].x; // to make code look cleaner
                    int adjY = adjacentCoordinates[i].y; 
                    // if this tile has not been visited:
                    if (std::find(open_set.begin(), open_set.end(), adjacentCoordinates[i]) == open_set.end())
                    {
                        // we found are destination
                        if (adjX == xDest && adjY == yDest)
                        {
                            evaluatedCoords[xDest][yDest].comeFrom.x = currentTile.x;
                            evaluatedCoords[adjX][adjY].comeFrom.y = currentTile.y;
                            destinationReached = true;
                            break;
                        }

                        // see if found tiles are valid through the whole unit width
                        bool validTiles = true;
                        for (int newX = adjX; newX < adjX + unit_w; newX++)
                        {
                            // coordinates are valid?
                            if (areValidCoordinates(newX, adjY, w, h) == false)
                            {
                                validTiles = false;
                                break;
                            }

                            // field can be visited?
                            if (checkFunc(newX, adjY) == false)
                            {
                                validTiles = false;
                                break;
                            }
                        }
                        
                        // if newfound tile is OK:
                        // we have an unocupied field
                        if (validTiles)
                        {
                            // say that we've came to evaluated coordinates from the current tile
                            evaluatedCoords[adjX][adjY].comeFrom.x = currentTile.x;
                            evaluatedCoords[adjX][adjY].comeFrom.y = currentTile.y;
                            evaluatedCoords[adjX][adjY].G = evaluatedCoords[currentTile.x][currentTile.y].G + 1;
                            open_set.push_back(adjacentCoordinates[i]);
                        }
                    }
                    else
                    {
                        // if we visited this tile already
                        if (evaluatedCoords[adjX][adjY].G > evaluatedCoords[currentTile.x][currentTile.y].G + 1)
                        {
                            // but the path would be shorter if we moved to this tile
                            // from the other one
                            evaluatedCoords[adjX][adjY].comeFrom.x = currentTile.x;
                            evaluatedCoords[adjX][adjY].comeFrom.y = currentTile.y;
                            evaluatedCoords[adjX][adjY].G = evaluatedCoords[currentTile.x][currentTile.y].G + 1;
                        }
                    }
                }
            }
        }

        // move the current tile to visited
        closed_set.push_back(currentTile);
        // remove it from unvisited tiles
        open_set.erase(open_set.begin() + tileNum);
    }

    if (destinationReached)
    {
        // let's make a path
        // go backwards from destination
        tile_list path;
        if (include_destination)
        {
            path.push_back({ xDest, yDest });
        }

        TileCoords currentTile = evaluatedCoords[xDest][yDest].comeFrom;

        // ... until we reach the starting tile
        while (currentTile.x != xStart || currentTile.y != yStart)
        {
            path.push_back(currentTile);
            currentTile = evaluatedCoords[currentTile.x][currentTile.y].comeFrom;
        }

        return path;
    }
    else
    {
        // could not find a way - return nothing
        return {};
    }
}


#endif

/*
Author: Vladimir Slav

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string.h>

#include "collisiongrid.h"
#include <algorithm>

Collideable::Collideable(double x, double y, int width, int height, int type, bool** CollisionGrid, size_t grid_tile_size)
	: x(x)
	, y(y)
	, width(width)
	, height(height)
	, type(type)
	, needsToBeDeleted(false)
{
	GridW = (width + grid_tile_size - 1) / grid_tile_size;
	GridH = (height + grid_tile_size - 1) / grid_tile_size;

	if (CollisionGrid == nullptr)
	{
		this->CollisionGrid = new bool*[GridW];
		for (size_t i = 0; i < GridW; i++)
		{
			this->CollisionGrid[i] = new bool[GridH];
			for (size_t j = 0; j < GridH; j++)
			{
				this->CollisionGrid[i][j] = true;
			}
		}
	}
}

bool Collideable::Collide(Collideable* target)
{
	return false;
}

bool Collideable::CollideWithBoundary()
{
	return false;
}

int Collideable::GetType() const
{
	return type;
}

Collideable::~Collideable()
{
	for (size_t i = 0; i < GridW; i++)
	{
		delete [] CollisionGrid[i];
	}
	delete[] CollisionGrid;
}


int Collideable::GetWidth() const
{
	return width;
}

int Collideable::GetHeight() const
{
	return height;
}

size_t Collideable::GetGridW() const
{
	return GridW;
}

size_t Collideable::GetGridH() const
{
	return GridH;
}

bool** Collideable::GetGrid() const
{
	return CollisionGrid;
}


double Collideable::GetX() const
{
	return x;
}

double Collideable::GetY() const
{
	return y;
}

void Collideable::SetCoords(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Collideable::Move(double dx, double dy)
{
	this->x += dx;
	this->y += dy;
}

bool Collideable::ShouldBeDeleted()
{
	return needsToBeDeleted;
}

void Collideable::DeleteOnNextCheck()
{
    needsToBeDeleted = true;
}

CollisionGrid::CollisionGrid(int top_border, int left_border, int w_in_pixels, int h_in_pixels, size_t square_w, size_t square_h)
	: wPixels(w_in_pixels)
	, hPixels(h_in_pixels)
	, squareWidth(square_w)
	, squareHeight(square_h)
	, wSquares(w_in_pixels / square_w + ((w_in_pixels % square_w > 0) ? 1 : 0))
	, hSquares(h_in_pixels / square_h + ((h_in_pixels % square_w > 0) ? 1 : 0))
	, topBorder(top_border)
	, leftBorder(left_border)
{
	objMap = new Collideable**[wSquares];
	for (size_t i = 0; i < wSquares; i++)
	{
		objMap[i] = new Collideable*[hSquares];
		memset(objMap[i], 0, sizeof(Collideable*) * hSquares);
	}
}

CollisionGrid::~CollisionGrid()
{
	for (size_t i = 0; i < wSquares; i++)
	{
		delete[] objMap[i];
	}
	delete[] objMap;

	for (auto obj : objects)
	{
		delete obj;
	}
}

bool CollisionGrid::Move(Collideable* obj, double x, double y)
{
	bool collidesWithBoundary = CollidesWithBoundary(obj, x, y);

	if (collidesWithBoundary == false)
	{
		obj->SetCoords(x, y);
		return true;
	}

	if (collidesWithBoundary)
	{
		obj->CollideWithBoundary();
	}

	return false;
}

bool CollisionGrid::CollidesWithBoundary(Collideable* obj, double nx, double ny)
{
	return (nx + obj->GetWidth() < wPixels &&
		    ny + obj->GetHeight() < hPixels &&
		    nx >= leftBorder &&
		    ny >= topBorder) == false;
}

bool CollisionGrid::Check(Collideable* obj, double nx, double ny)
{
	return CollidesWithBoundary(obj, nx, ny);
}

void CollisionGrid::AddObject(Collideable* new_obj)
{
	objects.push_back(new_obj);
}

std::vector<Collideable*>* CollisionGrid::GetObjects()
{
	return &objects;
}

void CollisionGrid::CheckCollissions(bool do_cleanup)
{
	size_t i = 0;
	while (i < objects.size())
	{
		bool collideWithBoundary = CollidesWithBoundary(objects[i], objects[i]->GetX(), objects[i]->GetY());
		if (collideWithBoundary)
		{
			if (objects[i]->CollideWithBoundary())
			{
				delete objects[i];
				objects.erase(objects.begin() + i);
				continue;
			}
		}

		i++;
	}

	for (i = 0; i < wSquares; i++)
	{
		memset(objMap[i], 0, sizeof(Collideable*) * hSquares);
	}

	i = 0;
	while (i < objects.size())
	{
		bool** gridMap = objects[i]->GetGrid();
		size_t objW = objects[i]->GetGridW();
		size_t objH = objects[i]->GetGridH();
		int initialX = std::max((static_cast<int>(objects[i]->GetX()) - leftBorder) / static_cast<int>(squareWidth), 0);
		for (int x = initialX; x < std::min(initialX + static_cast<int>(objW), static_cast<int>(wSquares) - 1); x++)
		{
			int initialY = std::max((static_cast<int>(objects[i]->GetY()) - topBorder) / static_cast<int>(squareHeight), 0);
			for (int y = initialY; y < std::min(initialY + static_cast<int>(objH), static_cast<int>(hSquares) - 1); y++)
			{
				if (gridMap[x - initialX][y - initialY])
				{
					if (objMap[x][y] == nullptr)
					{
						objMap[x][y] = objects[i];
					}
					else
					{
						if (objects[i]->ShouldBeDeleted() == false && objMap[x][y]->ShouldBeDeleted() == false)
						{
							objects[i]->Collide(objMap[x][y]);
							objMap[x][y]->Collide(objects[i]);
						}
					}
				}
			}
		}
		i++;
	}
	
    if (do_cleanup)
    {
        Cleanup();
    }
	//for (int i = 0; i < objects.size())
}

void CollisionGrid::Cleanup()
{
    size_t i = 0;
    while (i < objects.size())
    {
        if (objects[i]->ShouldBeDeleted())
        {
            delete objects[i];
            objects.erase(objects.begin() + i);
        }
        else
        {
            i++;
        }
    }

	for (i = 0; i < wSquares; i++)
	{
		memset(objMap[i], 0, sizeof(Collideable*) * hSquares);
	}
}

int CollisionGrid::GetTopBorder() const
{
    return topBorder;
}

Collideable* CollisionGrid::GetObjectFromCoordinates(int x, int y)
{
    int gridX = 0;
    int gridY = 0;

    GetGridCoordinates(x, y, &gridX, &gridY);
    //int gridX = (x - leftBorder) / squareWidth;
    //int gridY = (y - topBorder) / squareHeight;

    if (gridX >= 0 && gridX < static_cast<int>(wSquares) && 
        gridY >= 0 && gridY < static_cast<int>(hSquares))
    {
        return objMap[gridX][gridY];
    }

    return nullptr;
}

Collideable* CollisionGrid::GetObjectFromGridCoordinates(int x, int y)
{

    if (x >= 0 && x < static_cast<int>(wSquares) &&
        y >= 0 && y < static_cast<int>(hSquares))
    {
        return objMap[x][y];
    }

    return nullptr;
}

void CollisionGrid::GetGridCoordinates(int mousex, int mousey, int* x, int *y)
{
    if (x == nullptr || y == nullptr)
    {
        return;
    }

    *x = (mousex - leftBorder) / squareWidth;
    *y = (mousey - topBorder) / squareHeight;
}
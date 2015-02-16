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

/*
 Grid: for now, no strict coordinate checking. The idea is to split the grid into multiple small squares
 And if there is a unit in the square when another moves in - immediately send a collision command
*/

#ifndef __COLLISIONGRID_H__
#define __COLLISIONGRID_H__

#include <vector>

class CollisionGrid;

class Collideable
{
protected:
	double x;
	double y;

	int width;
	int height;

	int type;

	bool** CollisionGrid; // for non-square objects, to know which squares should be checked and which should not

	size_t GridW;
	size_t GridH;

	bool needsToBeDeleted;

public:
	Collideable(double x, double y, int width, int height, int type, bool** CollisionGrid, size_t grid_tile_size);
	virtual bool Collide(Collideable* target); // return true if object needs to be destroyed
	virtual bool CollideWithBoundary(); // return true if object needs to be destroyed
	~Collideable();

	int GetType() const;
	
	virtual int GetWidth() const;
	virtual int GetHeight() const;

	double GetX() const;
	double GetY() const;

	void SetCoords(double x, double y);
	void Move(double dx, double dy);

	bool** GetGrid() const;
	size_t GetGridW() const;
	size_t GetGridH() const;

	bool ShouldBeDeleted();
    void DeleteOnNextCheck();
};

class CollisionGrid
{
protected:
	//in case we don't have the full square @ the right or @the bottom:
	int wPixels;
	int hPixels;

	int topBorder;
	int leftBorder;

	size_t wSquares;
	size_t hSquares;

	size_t squareWidth;
	size_t squareHeight;

	Collideable*** objMap;

	std::vector<Collideable*> objects;

public:
	CollisionGrid(int top_border, int left_border, int w_in_pixels, int h_in_pixels, size_t square_w, size_t square_h);

	virtual void CheckCollissions(bool do_cleanup = true);
    virtual void Cleanup();

	std::vector<Collideable*>* GetObjects();

	virtual void AddObject(Collideable* new_obj);

	bool Move(Collideable* obj, double x, double y);
	bool Check(Collideable* obj, double nx, double ny);

	bool CollidesWithBoundary(Collideable* obj, double nx, double ny);
    int GetTopBorder() const;

    virtual Collideable* GetObjectFromCoordinates(int x, int y);
    virtual Collideable* GetObjectFromGridCoordinates(int x, int y);
    virtual void GetGridCoordinates(int mousex, int mousey, int* x, int *y);

	~CollisionGrid();
};

#endif
#pragma once
#include "GameObject.h"
class Unit
{
	friend class Grid;
private:
	Grid* grid;
	LPGAMEOBJECT obj;
	float x;
	float y;
	Unit* prev;
	Unit* next;
public:
	Unit(Grid* grid, LPGAMEOBJECT obj, float x, float y);
	void Move(float x, float y);
	LPGAMEOBJECT GetObj(){ return this->obj; }
};

class Grid
{
	int mapWidth;
	int mapHeight;
	
	int cellWidth;
	int cellHeight;

	int numCols;
	int numRows;

	vector <vector<Unit*>> cells;
public:
	Grid(int mapWidth, int mapHeight, int cellWidth, int cellHeight);
	~Grid();

	void Add(Unit* unit);
	void Move(Unit* unit, float x, float y);
	void Get(D3DVECTOR camPosition, vector<Unit*>& listUnits);
	void Out();
};


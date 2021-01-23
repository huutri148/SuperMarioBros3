﻿#include "Grid.h"
#include "Utils.h"

Unit::Unit(Grid* grid, LPGAMEOBJECT obj,
	float x, float y)
{
	this->grid = grid;
	this->obj = obj;
	this->x = x;
	this->y = y;

	this->prev = NULL;
	this->next = NULL;
	
	grid->Add(this);
}

Unit::Unit(Grid* grid, LPGAMEOBJECT obj, int gridRow, int gridCol)
{
	this->grid = grid;
	this->obj = obj;
	this->x = obj->x;
	this->y = obj->y;

	this->prev = NULL;
	this->next = NULL;

	grid->Add(this,gridRow,gridCol);
}

void Unit::Move(float x, float y)
{
	grid->Move(this, x, y);

}

Grid::Grid(int mapWidth, int mapHeight, int cellWidth, int cellHeight)
{
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;


	numCols = mapWidth / cellWidth;
	numRows = mapHeight / cellHeight;

	cells.resize(numCols);

	for (int i = 0; i < numCols; i++)
		cells[i].resize(numCols);

	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++)
			cells[i][j] = NULL;

}
Grid::~Grid()
{

}
void Grid::Add(Unit* unit)
{
	int row = (int)(unit->y / CELL_HEIGHT);
	int col = (int)(unit->x / CELL_WIDTH);
	 
	// Lame way 
	// Todo: Cần cách chia cell hợp lí hơn
	if (row == this->numRows)
		row = this->numRows - 1;
	if (col == this->numCols)
		col = this->numCols - 1;

	// thêm vào đầu cell - add head
	unit->prev = NULL;
	unit->next = cells[row][col];
	cells[row][col] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}
void Grid::Add(Unit* unit, int gridRow, int gridCol)
{
	// Lame way 
	// Todo: Cần cách chia cell hợp lí hơn
	if (gridRow == this->numRows)
		gridRow = this->numRows - 1;
	if (gridCol == this->numCols)
		gridCol = this->numCols - 1;

	// thêm vào đầu cell - add head
	unit->prev = NULL;
	unit->next = cells[gridRow][gridCol];
	cells[gridRow][gridCol] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}

void Grid::Move(Unit* unit, float x, float y)
{
	// lấy chỉ số cell cũ
	int oldRow = (int)(unit->y / CELL_HEIGHT);
	int oldCol = (int)(unit->x / CELL_WIDTH);

	// lấy chỉ số cell mới
	int newRow = (int)(y / CELL_HEIGHT);
	int newCol = (int)(x / CELL_WIDTH);

	// nếu object ra khỏi vùng viewport-> không cần cập nhật 
	if (newRow < 0 || newRow >= numRows || newCol < 0 ||
		newCol >= numCols)
	{
		return;
	}
	

	// cập nhật tọa độ mới
	unit->x = x;
	unit->y = y;

	// cell không thay đổi
	if (oldRow == newRow && oldCol == newCol)
		return;

	// hủy liên kết với cell cũ
	if (unit->prev != NULL)
	{
		unit->prev->next = unit->next;
	}
	if(unit->next != NULL)
	{
		unit->next->prev = unit->prev;
	}
	if (cells[oldRow][oldCol] == unit)
		cells[oldRow][oldCol] = unit->next;

	// thêm vào cell mới
	Add(unit);
}

void Grid::Get(float cam_x,float cam_y, vector<Unit*>& listUnits)
{
	int startCol = (int)(cam_x / CELL_WIDTH);
	int endCol =(int) ceil((cam_x + SCREEN_WIDTH) / CELL_WIDTH);
	int ENDCOL = (int)ceil((mapWidth) / CELL_WIDTH);
	if (endCol > ENDCOL)
		endCol = ENDCOL;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = startCol; j < endCol; j++)
		{
			Unit* unit = cells[i][j];
			while (unit != NULL)
			{
				if (unit->GetObj()->IsEnable() == true)
					listUnits.push_back(unit);
				unit = unit->next;
			}
		}
	}
}
void Grid::Out()
{
	int c = 0;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			
			Unit* unit = cells[i][j];
			while (unit)
			{
				c++;
				unit = unit->next;
			}
		}
	}
	DebugOut(L"\nNumber of Unit: %d", c);
}


Grid::Grid(int gridCols, int gridRows)
{
	this->numCols = gridCols;
	this->numRows = gridRows;

	this->mapWidth = gridCols * CELL_WIDTH;
	this->mapHeight = gridRows * CELL_HEIGHT;


	cells.resize(numCols);

	for (int i = 0; i < numCols; i++)
		cells[i].resize(numCols);

	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++)
			cells[i][j] = NULL;
}

void Grid::ClearAll()
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{

			Unit* unit = cells[i][j];
			while (unit)
			{
				if (cells[i][j] == unit)
					cells[i][j] = unit->next;
				if (unit->next != NULL)
					unit->next->prev = unit->prev;
				if (unit->prev != NULL)
					unit->prev->next = unit->next;
				Unit* tmp = unit;
				unit = unit->next;
				if(tmp->GetObj() != NULL)
					delete tmp->GetObj();
				delete tmp;
			}
		}
	}

}
#include "Grid.h"
#include "Utils.h"
#include"Item.h"
#include "FireBall.h"
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

void Unit::Move(float x, float y)
{
	grid->Move(this, x, y);

}

Grid::Grid(int mapWidth, int mapHeight, int cellWidth, int cellHeight)
{
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;

	this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;

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
	int row = (int)(unit->y / cellHeight);
	int col = (int)(unit->x / cellWidth);
	// thêm vào đầu cell - add head
	unit->prev = NULL;
	unit->next = cells[row][col];
	cells[row][col] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}
void Grid::Move(Unit* unit, float x, float y)
{
	// lấy chỉ số cell cũ
	int oldRow = (int)(unit->y / cellHeight);
	int oldCol = (int)(unit->x / cellWidth);

	// lấy chỉ số cell mới
	int newRow = (int)(y / cellHeight);
	int newCol = (int)(x / cellWidth);

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
	int startCol = (int)(cam_x / cellWidth);
	int endCol =(int) ceil((cam_x + SCREEN_WIDTH) / cellWidth);
	int ENDCOL = (int)ceil((mapWidth) / cellWidth);
	if (endCol >= ENDCOL)
		endCol -= 1;
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
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			int c = 0;
			Unit* unit = cells[i][j];
			while (unit)
			{
				c++;
				unit = unit->next;
			}
		}
	}
}

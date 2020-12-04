#include <fstream>
#include <iostream>
#include "Map.h"
#include"Utils.h"
Map::Map(int idMap, int nTitleCols, int nTitleRows, int nMapCols,
	int nMapRows, int nTotalTiles)
{
	this->TileSet = Textures::GetInstance()->Get(idMap);
	this->TotalColsOfMap = nMapCols;
	this->TotalRowsOfMap = nMapRows;
	this->TotalRowsOfTileSet = nTitleRows;
	this->TotalColsOfTitleSet = nTitleCols;
	this->TotalTiles = nTotalTiles;
}
Map::~Map()
{

}
void Map::CreateTilesFromTileSet()
{
	int left, bottom, right, top;
	for (int tileNum = 0; tileNum < TotalTiles; tileNum++) {
		left = tileNum % TotalColsOfTitleSet * TILE_WIDTH;
		top = tileNum / TotalColsOfTitleSet * TILE_HEIGHT;
		bottom = top + TILE_HEIGHT;
		right = left + TILE_WIDTH;
		LPSPRITE Tile = new Sprite(tileNum, left, top,
			right, bottom, TileSet);
		this->Tiles.push_back(Tile);
	}
}
void Map::LoadMatrix(LPCWSTR path)
{
	ifstream file;

	file.open(path);
	this->Matrix = new int* [TotalRowsOfMap];
	for (int iRow = 0; iRow < TotalRowsOfMap; iRow++)
	{
		this->Matrix[iRow] = new int[TotalColsOfMap];
		for (int iColumn = 0; iColumn < TotalColsOfMap; iColumn++)
		{
			file >> this->Matrix[iRow][iColumn];
		}

	}
	file.close();
}
void Map::Render(float cam_x,float cam_y, int screenWidth,int screenHeight)
{
	int FirstColumn =(int) floor(cam_x/ TILE_WIDTH);
	int FirstRow =(int) floor(cam_y / TILE_HEIGHT);
	// i don't really know what the f*** iam writting
	int LastRow =(int)floor( (cam_y + screenHeight - 24) / TILE_HEIGHT);
	int LastColumn =(int) ceil((cam_x + screenWidth ) / TILE_WIDTH);
	if (LastColumn >= TotalColsOfMap)
		LastColumn = TotalColsOfMap - 1;
	if (LastRow >= TotalRowsOfMap)
		LastRow = TotalRowsOfMap - 1;
	for (int iRow = FirstRow; iRow < LastRow; iRow++)
	{
		for (int iColumn = FirstColumn; iColumn < LastColumn; iColumn++)
		{
			this->Tiles[this->Matrix[iRow][iColumn] - 1]->Draw(-1,
				(float)iColumn * TILE_WIDTH,
				(float) iRow * TILE_HEIGHT);
		}
	}
}
int Map::GetMapHeiht()
{
	if (Height == 0)
		Height = TotalRowsOfMap * TILE_HEIGHT;
	return Height;
}
int Map::GetMapWidth()
{
	if (Width == 0)
	{
		Width = TotalColsOfMap * TILE_WIDTH;
	}
	return Width;
}

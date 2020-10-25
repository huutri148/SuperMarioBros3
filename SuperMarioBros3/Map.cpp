#include <fstream>
#include <iostream>
#include "Map.h"
#include"debug.h"
Map::Map(int idMap, int nTitleCols, int nTitleRows, int nMapCols, int nMapRows, int nTotalTiles)
{
	this->TileSet = CTextures::GetInstance()->Get(idMap);
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
		LPSPRITE Tile = new CSprite(tileNum, left, top, right, bottom, TileSet);
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
void Map::Render()
{
	for (int iRow = 0; iRow < TotalRowsOfMap; iRow++)
	{
		for (int iColumn = 0; iColumn < TotalColsOfMap; iColumn++)
		{
			this->Tiles[this->Matrix[iRow][iColumn] - 1]->Draw(iColumn * TILE_WIDTH, iRow * TILE_HEIGHT);
		}
	}
}

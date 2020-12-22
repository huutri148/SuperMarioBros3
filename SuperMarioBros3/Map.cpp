#include <fstream>
#include <iostream>
#include "Map.h"
#include"Utils.h"
Map::Map(int idMap, int nTitleCols, int nTitleRows, int nMapCols,
	int nMapRows, int nTotalTiles, float edgeLeft,float edgeRight,
	float edgeTop, float edgeBottomInWorld, 
	float edgeLeftInExtraMap, float edgeRightInExtraMap,
	float edgeTopInExtraMap,  float edgeBottomInExtraMap,
	float startPositionX, float startPositionY,
	float extraMapPositionX , float extraMapPositionY ,
	float worldMapPositionX , float worldMapPositionY )
{
	this->tileSet = Textures::GetInstance()->Get(idMap);

	this->totalColsOfMap = nMapCols;
	this->totalRowsOfMap = nMapRows;

	this->totalRowsOfTileSet = nTitleRows;
	this->totalColsOfTitleSet = nTitleCols;

	this->totalTiles = nTotalTiles;

	this->edgeLeft = edgeLeft;
	this->edgeRight = edgeRight;
	this->edgeBottomInWorld = edgeBottomInWorld;	
	this->edgeTop = edgeTop;

	this->edgeLeftInExtraMap = edgeLeftInExtraMap;
	this->edgeRightInExtraMap = edgeRightInExtraMap;
	this->edgeTopInExtraMap = edgeTopInExtraMap;
	this->edgeBottomInExtraMap = edgeBottomInExtraMap;

	this->startPositionX = startPositionX;
	this->startPositionY = startPositionY; 

	this->extraMapPositionX = extraMapPositionX;
	this->extraMapPositionY = extraMapPositionY;

	this->worldMapPositionX = worldMapPositionX;
	this->worldMapPositionY = worldMapPositionY;
}
Map::~Map()
{

}
void Map::CreateTilesFromTileSet()
{
	int left, bottom, right, top;
	for (int tileNum = 0; tileNum < totalTiles; tileNum++) {
		left = tileNum % totalColsOfTitleSet * TILE_WIDTH;
		top = tileNum / totalColsOfTitleSet * TILE_HEIGHT;
		bottom = top + TILE_HEIGHT;
		right = left + TILE_WIDTH;
		LPSPRITE Tile = new Sprite(tileNum, left, top,
			right, bottom, tileSet);
		this->tiles.push_back(Tile);
	}
}
void Map::LoadMatrix(LPCWSTR path)
{
	ifstream file;

	file.open(path);
	this->matrix = new int* [totalRowsOfMap];
	for (int iRow = 0; iRow < totalRowsOfMap; iRow++)
	{
		this->matrix[iRow] = new int[totalColsOfMap];
		for (int iColumn = 0; iColumn < totalColsOfMap; iColumn++)
		{
			file >> this->matrix[iRow][iColumn];
		}

	}
	file.close();
}
void Map::Render(float cam_x,float cam_y, 
				int screenWidth,int screenHeight,
				float translateX, float translateY)
{
	int FirstColumn =(int) floor(cam_x/ TILE_WIDTH);
	int LastColumn = (int)ceil((cam_x + screenWidth) / TILE_WIDTH);

	int FirstRow =(int) floor(cam_y / TILE_HEIGHT);
	int LastRow =(int)floor( (cam_y + screenHeight - 24) / TILE_HEIGHT);
	

	float transX = floor(translateX);
	float transY = floor(translateY);

	//DebugOut(L"\nF: %d - L:  %d", FirstColumn, LastColumn);
	if (LastColumn >= totalColsOfMap)
		LastColumn = totalColsOfMap ;
	if (LastRow >= totalRowsOfMap)
		LastRow = totalRowsOfMap ;


	for (int iRow = FirstRow; iRow < LastRow; iRow++)
	{
		for (int iColumn = FirstColumn; iColumn < LastColumn; iColumn++)
		{
			this->tiles[this->matrix[iRow][iColumn] - 1]->Draw(-1,
				(float)iColumn * TILE_WIDTH,
				(float) iRow * TILE_HEIGHT,
				255, transX, transY);
		}
	}
}
int Map::GetMapHeiht()
{
	if (height == 0)
		height = totalRowsOfMap * TILE_HEIGHT;
	return height;
}
int Map::GetMapWidth()
{
	if (width == 0)
	{
		width = totalColsOfMap * TILE_WIDTH;
	}
	return width;
}

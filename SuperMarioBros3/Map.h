#pragma once
#include <d3d9.h>
#include"Textures.h"
#include "Sprites.h"
#include"Define.h"

class Map
{
	int** matrix;
	int totalColsOfTitleSet, totalRowsOfTileSet;
	int totalColsOfMap, totalRowsOfMap;
	int totalTiles;
	LPDIRECT3DTEXTURE9 tileSet;
	vector<LPSPRITE> tiles;
	int height = 0;
	int width = 0;

	

public: 

	float edgeLeft;
	float edgeRight;
	float edgeTop;
	float edgeBottomInWorld;
	float edgeBottomInExtraMap;
	float edgeLeftInExtraMap;
	float edgeRightInExtraMap;
	float edgeTopInExtraMap;
	float startPositionX, startPositionY;
	float extraMapPositionX, extraMapPositionY;
	float worldMapPositionX, worldMapPositionY;

	Map(int idMap, int nTitleCols,
		int nTitleRows, int nMapCols, int nMapRows,
		int nTotalTiles, float edgeLeft, float edgeRight,
		float edgeBottomInWorld, float edgeTop,
		float edgeLeftInExtraMap,
		float edgeRightInExtraMap,
		float edgeBottomInExtraMap,
		float edgeTopInExtraMap,
		float startPositionX, float startPositionY,
		float extraMapPositionX = 0, float startMapPositionY = 0,
		float worldMapPositionX = 0, float worldMapPositionY = 0);

	~Map();

	void CreateTilesFromTileSet();
	void LoadMatrix(LPCWSTR path);
	void Render(float cam_x, float cam_y,int screenWidth,int screenHeight
	,float translateX = 0.0f, float translateY = 0.0f);
	int GetTotalColsOfMap() { return this->totalColsOfMap; }
	int GetTotalRowsOfMap() { return this->totalRowsOfMap; }
	int GetMapHeiht();
	int GetMapWidth();
};


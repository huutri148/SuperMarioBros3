#pragma once
#include <d3d9.h>
#include"Textures.h"
#include "Sprites.h"
#include"Define.h"

class Map
{
	int** Matrix;
	int TotalColsOfTitleSet, TotalRowsOfTileSet;
	int TotalColsOfMap, TotalRowsOfMap;
	int TotalTiles;
	LPDIRECT3DTEXTURE9 TileSet;
	vector<LPSPRITE> Tiles;
	int Height = 0;
	int Width = 0;

	

public: 

	//Todo : add information in PlayScene and World_obj.txt
	float edgeLeft;
	float edgeRight;
	float edgeBottomInWorld;
	float edgeBottomInExtraMap;

	Map(int idMap, int nTitleCols, int nTitleRows, int nMapCols, int nMapRows, int nTotalTiles, float edgeLeft,float edgeRight,
		float edgeBottomInWorld = 0, float edgeBottomInExtraMap = 0);
	~Map();
	void CreateTilesFromTileSet();
	void LoadMatrix(LPCWSTR path);
	void Render(float cam_x, float cam_y,int screenWidth,int screenHeight
	,float translateX = 0.0f, float translateY = 0.0f);
	int GetTotalColsOfMap() { return this->TotalColsOfMap; }
	int GetTotalRowsOfMap() { return this->TotalRowsOfMap; }
	int GetMapHeiht();
	int GetMapWidth();
};


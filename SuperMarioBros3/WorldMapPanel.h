#pragma once
#include"GameObject.h"
class WorldMapPanel : public GameObject
{
	int panelId;
	int* possibleDirect;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable) {};
	virtual void Render(float translateX = 0, float translateY = 0);
	WorldMapPanel(int panelId, int Up,int Down,int Right, int Left);
};


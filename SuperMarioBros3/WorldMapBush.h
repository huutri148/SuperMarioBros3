#pragma once
#include"GameObject.h"

class WorldMapBush :public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable) {};
	virtual void Render(float translateX = 0, float translateY = 0);
	virtual int GetWidth() { return 0; };
};


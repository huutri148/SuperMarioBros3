#pragma once
#include "GameObject.h"
#define BLOCK_BBOX_WIDTH  32
#define BLOCK_BBOX_HEIGHT 16
class Pipe :public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
};


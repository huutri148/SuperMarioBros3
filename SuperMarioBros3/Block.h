#pragma once
#include"GameObject.h"
#include"Define.h"
class Block : public GameObject
{
	float width;
	float height;
public:
	Block(float x, float y, float width = BLOCK_BBOX_WIDTH, float height = BLOCK_BBOX_HEIGHT);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual int GetWidth() { return (int)width; };
};



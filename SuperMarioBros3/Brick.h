#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16


class Brick : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b,bool isEnable);
};

#define INVISIBLEBRICK_TYPE_GROUND	0
#define INVISIBLEBRICK_TYPE_BLOCK	1


class InvisibleBrick : public GameObject
{
private:
	int type;
public:
	InvisibleBrick(int _type = 0) { type = _type; };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	int GetType() { return type; };
};
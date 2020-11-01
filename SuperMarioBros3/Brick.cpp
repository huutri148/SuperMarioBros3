#include "Brick.h"

void Brick::Render()
{
	animation_set->at(0)->Render(-1,x, y);
	//RenderBoundingBox();
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b,bool isEnable)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void InvisibleBrick::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}

void InvisibleBrick::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
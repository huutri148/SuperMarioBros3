#include "Ground.h"

void Ground::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + GROUND_BBOX_WIDTH;
	b = y + GROUND_BBOX_HEIGHT;
	//r = x + width;
	//b = y + height;
}
//Ground::Ground(float x, float y, float width, float height)
//{
//	this->x = x;
//	this->y = y;
//	this->width = width;
//	this->height = height;
//}

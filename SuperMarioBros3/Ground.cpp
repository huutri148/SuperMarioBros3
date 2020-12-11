#include "Ground.h"

void Ground::Render()
{
	//animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = l + width;
	b = t + height;
}

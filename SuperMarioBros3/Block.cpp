#include "Block.h"

void Block::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}

void Block::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + BLOCK_BBOX_WIDTH;
	b = y + BLOCK_BBOX_HEIGHT;
}

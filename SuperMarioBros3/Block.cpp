#include "Block.h"

void Block::Render()
{
	//RenderBoundingBox();
}
Block::Block(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Block::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

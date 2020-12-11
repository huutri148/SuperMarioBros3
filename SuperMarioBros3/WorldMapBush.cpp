#include "WorldMapBush.h"



void WorldMapBush::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}
void WorldMapBush::Render(float translateX, float translateY)
{
	animation_set->at(0)->Render(-1, x, y,255,translateX, translateY);
	//RenderBoundingBox();
}
void WorldMapBush::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = this->x;
	t = this->y;
	r = x + WORLDMAP_BUSH_BBOX_WIDTH;
	b = y + WORLDMAP_BUSH_BBOX_HEIGHT;
}

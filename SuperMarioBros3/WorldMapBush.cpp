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

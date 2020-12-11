#include "WorldMapPanel.h"
void WorldMapPanel::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}
void WorldMapPanel::Render(float translateX, float translateY)
{
	animation_set->at(0)->Render(-1, x, y, 255, translateX, translateY);
	//RenderBoundingBox();
}

WorldMapPanel::WorldMapPanel(int left, int top, int right, int bottom,int sceneId, int type)
{
	this->sceneId = sceneId;
	this->type = type;
	possibleTop = top;
	possibleLeft = left;
	possibleRight = right;
	possibleBottom = bottom;
}
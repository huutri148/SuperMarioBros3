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
//WorldMapPanel::WorldMapPanel(int sceneId, int Up,int Down,int Right,int Left)
//{
//	possibleDirect = new int[4];
//	this->sceneId = sceneId;
//	this->possibleDirect[0] = Up;
//	this->possibleDirect[1] = Down;
//	this->possibleDirect[2] = Right;
//	this->possibleDirect[3] = Left;
//}

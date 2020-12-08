#include "WorldMapPlayer.h"
void WorldMapPlayer::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}
void WorldMapPlayer::Render(float translateX, float translateY)
{
	animation_set->at(0)->Render(-1, round(x),round( y), 255, translateX, translateY);
	//RenderBoundingBox();
}
void WorldMapPlayer::Up()
{
	vy = -OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Down()
{
	vy = OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Left()
{
	vx = -OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Right()
{
	vx = OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}

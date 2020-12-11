#include "WorldMapPlayer.h"
#include"Utils.h"
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
	if(vx == 0 && vy == 0)
		vy = -OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Down()
{
	if (vx == 0 && vy == 0)
		vy = OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Left()
{
	if (vx == 0 && vy == 0)
		vx = -OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Right()
{
	if (vx == 0 && vy == 0)
		vx = OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult,
			min_tx, min_ty,
			nx, ny);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<WorldMapBush*>(e->obj))
			{
				DebugOut(L"\nAAAAAAAAAAA");
				if (e->ny != 0)
					vy = 0;
				if (e->nx != 0)
					vx = 0;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void WorldMapPlayer::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + 14;
	b = y + 14;
}

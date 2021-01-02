#include "RaccoonTail.h"
#include"Brick.h"
#include"Enemy.h"
void RaccoonTail::Render()
{
	RenderBoundingBox();
}

void RaccoonTail::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case RACCOONTAIL_STATE_ATTACK:
		vx = 0.06f * nx;
		vy = 0;
		break;
	case RACCOONTAIL_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}
void RaccoonTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (dt > 64)
		dt = 16;
	GameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
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
			if (dynamic_cast<Brick*>(e->obj))
			{
				dynamic_cast<Brick*>(e->obj)->SetEmpty(true);
				Game* game = Game::GetInstance();
				LPSCENE scene = game->GetCurrentScene();
				if (dynamic_cast<PlayScene*>(scene))
				{
					Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
					HitEffect* effect = new HitEffect(x, y);
					Unit* unit = new Unit(grid, effect, x, y);
				}
			}
			else if (dynamic_cast<Enemy*>(e->obj))
			{
				dynamic_cast<Enemy*>(e->obj)->SetBeingSkilled((int)(-e->nx));
				Game* game = Game::GetInstance();
				LPSCENE scene = game->GetCurrentScene();
				if (dynamic_cast<PlayScene*>(scene))
				{
					Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
					HitEffect* effect = new HitEffect(x, y);
					Unit* unit = new Unit(grid, effect, x, y);
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void RaccoonTail::GetBoundingBox(float& left, float& top,
	float& right, float& bottom, bool isEnable)
{
	left = x;
	top = y;
	right = left + RACCOONTAIL_BBOX_WIDTH;
	bottom = top + RACCOONTAIL_BBOX_HEIGHT;
}

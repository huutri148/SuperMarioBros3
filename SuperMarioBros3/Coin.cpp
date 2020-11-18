#include "Coin.h"
#include <algorithm>
#include "Mario.h"
#include"Brick.h"
void Coin::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	if (isEnable == true)
	{

		left = x;
		top = y;
		right = x + COIN_BBOX_WIDTH;
		bottom = y + COIN_BBOX_HEIGHT;
	}

	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}

void Coin::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	if (state != COIN_STATE_APPEAR)
		return;
	GameObject::Update(dt, coObjects);
	vy += RACCOONLEAF_GRAVITY * dt;
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
		float x0 = x, y0 = y;
		x = x0 + dx;
		y = y0 + dy;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Brick*>(e->obj))
			{
				if (e->ny < 0)
				{
					this->SetState(COIN_STATE_INACTIVE);
				}
				else
				{
					x = x0 + dx;
					y = y0 + dy;
				}
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Coin::Render()
{
	if (isEnable == true)
	{
		if (state != COIN_STATE_INACTIVE)
		{
			int ani;
			if(type == COIN_TYPE_3)
				ani = COIN_ANI_TYPE_3;
			if (type == COIN_TYPE_1)
				ani = COIN_ANI_TYPE_1;
			else
				ani = COIN_ANI_TYPE_2;
			animation_set->at(ani)->Render(-1, x, y);
		}
	}
	//RenderBoundingBox();
}
void Coin::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_IDLE:
		isEnable = true;
		vx = 0;
		vy = 0;
		nx = -1;
		break;
	case COIN_STATE_INACTIVE:
		isEnable = false;
		break;
	case COIN_STATE_APPEAR:
		vy = -COIN_SPEED_DELECT_Y;
		vx = 0;
		break;
	}
}
void Coin::Appear(float x, float y)
{
	this->SetPosition(x +  3, y);
	this->isEnable = true;
	this->SetState(COIN_STATE_APPEAR);
}
Coin::Coin(int type)
{
	this->type = type;
	if (type == COIN_TYPE_2)
		this->SetState(COIN_STATE_IDLE);
	if (type == COIN_TYPE_1)
		this->SetState(COIN_STATE_INACTIVE);
}
void Coin::Used()
{
	this->SetState(COIN_STATE_INACTIVE);
}

#include "Goomba.h"
#include "Pipe.h"
#include "Brick.h"
#include "Ground.h"
#include "Utils.h"
#include "Block.h"
#include <algorithm>

void Goomba::GetBoundingBox(float& left, float& top, 
	float& right, float& bottom,
	bool isEnable)
{
	if (isEnable == true)
	{

		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}

	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}

void Goomba::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	/*if (this->IsAbleToActive() == true)
	{
		this->SetPosition(entryX, entryY);
		this->SetState(GOOMBA_STATE_WALKING);
		isEnable = true;
	}*/
	HandleTimeSwitchState();
	if (state == GOOMBA_STATE_INACTIVE)
		return;

	Enemy::Update(dt, coObjects);
	if (this->state != GOOMBA_STATE_BEING_STROMPED)
		vy += dt * GOOMBA_GRAVITY;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if(!this->IsDead())
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
		/*x += min_tx * dx + nx * 0.4f;		
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
		{
			this->nx = -this->nx;
			vx = -vx;
		}

		if (ny < 0 && state != GOOMBA_STATE_DIE_NX) vy = 0;*/
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Pipe*>(e->obj) || 
				dynamic_cast<Brick*>(e->obj)||
				dynamic_cast<InvisibleBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					this->nx = -this->nx;
					vx = -vx;
					this->x = x0 + e->t * dx + e->nx * 0.4f;
				}
				else
				{
					x = x0 + dx;
					y = y0 + dy;
				}
			}
			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->nx != 0)
				{
					this->x = x0 + min_tx * dx + e->nx * 0.4f;
				}
				if (e->ny != 0)
				{
					vy = 0;
					this->y = y0 + min_ty * dy + e->ny * 0.4f;
				}
				
			}
		/*	if (dynamic_cast<Enemy*>(e->obj))
			{
				x = x0 + dx;
				y = y0 + dy;
			}*/
		
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Goomba::Render()
{
	if (isEnable == true)
	{
		if (state != GOOMBA_STATE_INACTIVE)
		{
			int ani;
			ani = GOOMBA_ANI_WALKING;
			if (state == GOOMBA_STATE_BEING_STROMPED) {
				ani = GOOMBA_ANI_DIE;
			}
			animation_set->at(ani)->Render(nx, x, y);
		}
	}
	//RenderBoundingBox();
}
void Goomba::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (dynamic_cast<Goomba*>(coObjects->at(i))
			|| dynamic_cast<Block*>(coObjects->at(i)))
		{
			continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(),
		CollisionEvent::compare);
}
void Goomba::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		nx = -1;
		vx = nx *GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_BEING_SKILLED:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = 0;
		break;
	case GOOMBA_STATE_BEING_STROMPED:
		y += GOOMBA_BBOX_HEIGHT -
			GOOMBA_BBOX_HEIGHT_DIE - 7;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_INACTIVE:
		vx = 0;
		isEnable = false;
		break;
	}
}
bool Goomba::IsDead()
{
	if (this->state == GOOMBA_STATE_BEING_STROMPED||
		this->state == GOOMBA_STATE_BEING_SKILLED || 
		this->state == GOOMBA_STATE_INACTIVE)
	{
		return true;
	}
	return false;
}
 

void Goomba::SetBeingStromped()
{
	this->SetState(GOOMBA_STATE_BEING_STROMPED);
	deathTime = GetTickCount();
}
void Goomba::SetBeingSkilled()
{
	this->SetState(GOOMBA_STATE_BEING_SKILLED);
	deathTime = GetTickCount();
}
void Goomba::EnableAgain()
{
	Enemy::EnableAgain();
	this->SetState(GOOMBA_STATE_WALKING);
}
void Goomba::HandleTimeSwitchState()
{
	if (GetTickCount64() - deathTime > GOOMA_INACTIVE_TIME &&
		this->IsDead() == true)
	{
		this->SetState(GOOMBA_STATE_INACTIVE);
		return;
	}
}

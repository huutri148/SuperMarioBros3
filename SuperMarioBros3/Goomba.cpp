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
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	HandleTimeSwitchState();
	if (state == GOOMBA_STATE_INACTIVE || isEnable == false)
		return;
	Enemy::Update(dt, coObjects);


	if (this->state != GOOMBA_STATE_BEING_STOMPED)
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Brick*>(e->obj) || dynamic_cast<Pipe*>(e->obj))
			{
				if (nx != 0)
				{
					this->ChangeDirect();
				}
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->nx != 0)
				{
					if(e->obj->y <= y)
						this->ChangeDirect();
				}
			}
			else if(!dynamic_cast<Mario*>(e->obj))
			{
				if(e->ny != 0)
					y -= (min_ty * dy + ny * 0.4f);
				if (e->ny < 0)
					this->SetBeingStomped();
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Goomba::Render()
{
	if (state != GOOMBA_STATE_INACTIVE && isEnable == true)
	{
		int ani;
		ani = GOOMBA_ANI_WALKING;
		if (state == GOOMBA_STATE_BEING_STOMPED)
			ani = GOOMBA_ANI_DIE;
		animation_set->at(ani)->Render(nx, ny, round(x), round(y));
	}
	//RenderBoundingBox();
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
		vy = -GOOMBA_DIE_DEFLECT_SPEED_Y;
		vx = nx * GOOMBA_DIE_DEFLECT_SPEED_X;
		isDead = true;
		ny = 1;
		break;
	case GOOMBA_STATE_BEING_STOMPED:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE ;
		isDead = true;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_INACTIVE:
		vx = 0;
		x = entryX;
		y = entryY;
		break;
	case GOOMBA_STATE_DEATH:
		isEnable = false;
		break;
	}
}
bool Goomba::IsDead()
{
	if (this->state != GOOMBA_STATE_WALKING)
	{
		return true;
	}
	return false;
}
 

void Goomba::SetBeingStomped()
{
	this->SetState(GOOMBA_STATE_BEING_STOMPED);
	Game* game = Game::GetInstance();
	LPSCENE scene = game->GetCurrentScene();
	this->GainScore(100);
	deathTime = GetTickCount();
}
void Goomba::SetBeingSkilled(int nx)
{
	this->nx = nx;
	this->SetState(GOOMBA_STATE_BEING_SKILLED);
	this->GainScore(100);
	deathTime = GetTickCount();
}
void Goomba::HandleTimeSwitchState()
{
	if (GetTickCount64() - deathTime > GOOMA_DEATH_TIME &&
		this->IsDead() == true && isDead == true)
	{
		this->SetState(GOOMBA_STATE_DEATH);
		return;
	}
}

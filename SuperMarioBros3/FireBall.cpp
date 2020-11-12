#include "FireBall.h"
#include "Ground.h"
#include "Goomba.h"
#include "Brick.h"

void FireBall::Render()
{
	if (state != FIREBALL_STATE_INACTIVE)
	{
		int ani;
		if (state == FIREBALL_STATE_NORMAL)
			ani = FIREBALL_ANI_NORMAL;
		else
			ani = FIREBALL_ANI_EXPLODE;
		animation_set->at(ani)->Render(-1, x, y);
		//RenderBoundingBox();
	}
}
void FireBall :: Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == FIREBALL_STATE_INACTIVE)
		return;
	if (GetTickCount() - explodeTime > FIREBALL_INACTIVE_TIME && this->state == FIREBALL_STATE_EXPLODE )
	{
		this->SetState(FIREBALL_STATE_INACTIVE);
		return;
	}
	GameObject::Update(dt);
	if(state == FIREBALL_STATE_NORMAL)
		vy += FIREBALL_GRAVITY;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state == FIREBALL_STATE_NORMAL)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nex = 0, ney;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nex, ney);

			// Collision logic 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy =0 - FIREBALL_SPEED_NY;
					this->y +=  min_ty * this->dy + ney * 0.1f;
				}
			/*	if (e->nx != 0)
				{
					this->SetState(FIREBALL_STATE_EXPLODE);
					explodeTime = GetTickCount();
				}*/
				else 
				{
					x += dx;
					y += dy;
				}
			}
			else
			{
				if (dynamic_cast<Enemy*>(e->obj))
				{
					Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
					if (enemy->IsDead() != true || !enemy->IsEnable() )
					{
						dynamic_cast<Enemy*>(e->obj)->SetBeingSkilled();
						this->SetState(FIREBALL_STATE_EXPLODE);
						explodeTime = GetTickCount();
					}
					else
					{
						x += dx;
						y += dy;
					}
				
				}
				else 	if (e->nx != 0)
				{
					this->SetState(FIREBALL_STATE_EXPLODE);
					explodeTime = GetTickCount();
				}
				else
				{
					x += dx;
					y += dy;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void FireBall ::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	if (isEnable == true)
	{
		l = x;
		t = y;
		r = x + FIREBALL_BBOX_WIDTH;
		b = y + FIREBALL_BBOX_HEIGHT;
	}
	else
	{
		l = 0;
		t = 0;
		r= 0;
		b = 0;
	}

}
void FireBall::SetState(int _state)
{
	this->state = _state;
	switch (_state)
	{
	case FIREBALL_STATE_NORMAL:
		this->vx = this->nx * FIREBALL_SPEED_NX;
		this->vy = FIREBALL_SPEED_NY;
		break;
	case FIREBALL_STATE_EXPLODE:
		vy = 0;
		vx = 0;
		break;
	case FIREBALL_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}

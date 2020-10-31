#include "Goomba.h"
#include "Pipe.h"
#include "Brick.h"
#include "Ground.h"
#include "Utils.h"
#include "Block.h"

void Goomba::GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable)
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

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (this->IsAbleToActive() == true)
	{
		this->SetPosition(entryX, entryY);
		this->SetState(GOOMBA_STATE_WALKING);
		isEnable = true;
	}*/
	if (state == GOOMBA_STATE_INACTIVE)
		return;
	if (GetTickCount() - time_death > GOOMA_INACTIVE_TIME && this->IsDead() == true)
	{
		this->SetState(GOOMBA_STATE_INACTIVE);
		return;
	}
	Enemy::Update(dt, coObjects);
	if (this->state != GOOMBA_STATE_DIE_NY)
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
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
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
			if (dynamic_cast<Pipe*>(e->obj) || dynamic_cast<Brick*>(e->obj)|| dynamic_cast<Block*>(e->obj))
			{
				DebugOut(L"\nBrick: nx: %f , ny: %f", e->nx, e->ny);
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
					this->y = y0 + min_ty * dx + e->ny * 0.4f;
				}
				
			}

		
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
//void Goomba::HandleCollision(float min_tx, float min_ty, float nex, float ney, float x0, float y0)
//{
//	if (nex != 0)
//	{
//		this->vx = 0;
//		this->x = x0 + min_tx * this->dx + nex * 0.1f;
//	}
//	if (ney != 0)
//	{
//		this->vy = 0;
//		this->y = y0 + min_ty * this->dy + ney * 0.1f;
//	}
//}
void Goomba::Render()
{
	if (state != GOOMBA_STATE_INACTIVE)
	{
		int ani;
		ani = GOOMBA_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE_NY) {
			ani = GOOMBA_ANI_DIE;
		}
		animation_set->at(ani)->Render(nx,x, y);
	}

	//RenderBoundingBox();
}

void Goomba::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE - 3;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = nx *GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_DIE_NX:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = 0;
		isEnable = false;
		break;
	case GOOMBA_STATE_DIE_NY:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE +3;
		isEnable = false;
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
	if (this->state == GOOMBA_STATE_DIE_NY|| this->state == GOOMBA_STATE_DIE_NX )
	{
		return true;
	}
	return false;
}
 
void Goomba::SetDie(bool n)
{
	if (n == true)
	{
		this->SetState(GOOMBA_STATE_DIE_NX);
	}
	else
		this->SetState(GOOMBA_STATE_DIE_NY);
	time_death = GetTickCount();
};
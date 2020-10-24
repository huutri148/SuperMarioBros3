#include "Goomba.h"

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable)
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

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
		// block 
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
			if (nx != 0)
			{
				
			}
			if (ny < 0 && state != GOOMBA_STATE_DIE_NX) vy = 0;
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CGoomba::Render()
{
	if (state != GOOMBA_STATE_INACTIVE)
	{
		int ani;
		ani = GOOMBA_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE_NY) {
			ani = GOOMBA_ANI_DIE;
		}
		animations[ani]->Render(x, y);
	}

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
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
		vx = -GOOMBA_WALKING_SPEED;
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
bool CGoomba::IsDead()
{
	if (this->state == GOOMBA_STATE_DIE_NY|| this->state == GOOMBA_STATE_DIE_NX )
	{
		return true;
	}
	return false;
}
 
void CGoomba::SetDie(bool n)
{
	if (n == true)
	{
		this->SetState(GOOMBA_STATE_DIE_NX);
	}
	else
		this->SetState(GOOMBA_STATE_DIE_NY);
	time_death = GetTickCount();
};
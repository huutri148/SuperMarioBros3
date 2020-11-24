#include "ParaGoomba.h"
#include "Block.h"
void ParaGoomba::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	if (isEnable == true)
	{
		left = x;
		top = y;
		right = x + PARAGOOMBA_BBOX_WIDTH;
		bottom = y + PARAGOOMBA_BBOX_HEIGHT;
	}
	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}

void ParaGoomba::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GOOMBA_STATE_INACTIVE)
		return;
	Enemy::Update(dt, coObjects);
	vy += dt * PARAGOOMBA_GRAVITY;
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
			if (!dynamic_cast<Block*>(e->obj))
			{
				if (nx != 0 && ny == 0)
				{
					this->ChangeDirect();
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void ParaGoomba::Render()
{
	if (isEnable)
	{
		int ani = 0;
		if (state == PARAGOOMBA_STATE_WALKING)
			ani = PARAGOOMBA_ANI_WALKING;
		else
			ani = PARAGOOMBA_ANI_JUMPING;
		animation_set->at(ani)->Render(nx, ny, x, y);
	}
}
void ParaGoomba::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case PARAGOOMBA_STATE_WALKING:
		vx =- PARAGOOMBA_WALKING_SPEED;
		nx = -1;
		switchStateTime = GetTickCount();
		vy = 0;
		break;
	case PARAGOOMBA_STATE_JUMPING:
		switchStateTime = GetTickCount();
		vy = -PARAGOOMBA_JUMP_SPEED;
		break;
	case PARAGOOMBA_STATE_SUPER_JUMPING:
		vy = -PARAGOOMBA_SUPER_JUMP_SPEED;
		break;
	}
}
bool ParaGoomba::IsDead()
{
	return true;
}


void ParaGoomba::SetBeingStromped()
{

}
void ParaGoomba::SetBeingSkilled(int nx)
{
}
void ParaGoomba::HandleTimeSwitchState()
{
}

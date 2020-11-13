#include "Mushroom.h"
#include"Game.h"
void Mushroom::Render()
{

	if (isEnable == true)
	{
		int ani = MUSHROOM_ANI_POWERUP;
		animation_set->at(ani)->Render(this->nx, x, y);
	}
	//RenderBoundingBox();
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r,
	float& b, bool isEnable)
{
	if (isEnable == true)
	{
		l = x;
		t = y;
		r = x + MUSHROOM_BBOX_WIDTH;
		b = y + MUSHROOM_BBOX_HEIGHT;
	}
	else
	{
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}

}
void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MUSHROOM_STATE_INACTIVE)
		return;
	GameObject::Update(dt, coObjects);
	vy += RACCOONLEAF_GRAVITY * dt;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (state != MUSHROOM_STATE_INACTIVE)
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
			if (dynamic_cast<Brick*>(e->obj)||
				dynamic_cast<Ground*>(e->obj) ||
				dynamic_cast<Block*>(e->obj))
			{
				if (e->ny < 0)
				{
					this->y = y0 + min_ty * dy + e->ny * 0.4f;
					vy = 0;
					this->SetState(MUSHROOM_STATE_WALKING);
				}
				else
				{
					x = x0 + dx;
					y = y0 + dy;
				}
			}
			 else if(dynamic_cast<Pipe*>(e->obj))
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
		
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Mushroom::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case MUSHROOM_STATE_APPEARANCE:
		isEnable = true;
		nx = 1;
		vx = 0;
		vy = -MUSHROOM_SPEED_APPEARANCE_Y;
		break;
	case MUSHROOM_STATE_INACTIVE:
		isEnable = false;
		vy = 0;
		vx = 0;
		break;
	case MUSHROOM_STATE_WALKING:
		vx = nx * MUSHROOM_SPEED_X;
		break;
	}
}
void Mushroom::Appear(float x, float y)
{
	this->SetPosition(x, y);
	this->SetState(RACCOONLEAF_STATE_APPEARANCE);
	this->isEnable = true;
	this->nx = 1;
}
void Mushroom::Used()
{
	this->SetState(MUSHROOM_STATE_INACTIVE);
	LPSCENE scence = Game::GetInstance()->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	mario->TurnBigForm();
	
}
Mushroom::Mushroom()
{
	isEnable = false;
}

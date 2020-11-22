#include "Mushroom.h"
#include"Game.h"
void Mushroom::Render()
{

	if (isEnable == true)
	{
		int ani = MUSHROOM_ANI_POWERUP;
		if (type == MUSHROOM_TYPE_1UP)
			ani = MUSHROOM_ANI_1UP;
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
	if (state == MUSHROOM_STATE_WALKING || y <= appearY)
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
		this->y += min_ty * dy + ny * 0.4f;
		this->x += min_tx * dx + nx * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Brick*>(e->obj))
			{
				if (e->ny < 0)
				{
					this->SetState(MUSHROOM_STATE_WALKING);
					vy = 0;
				}
			}
			else if (!dynamic_cast<Block*>(e->obj))
			{
				if (nx != 0 && ny == 0)
				{
					this->vx = -this->vx;
					this->nx = -this->nx;
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
void Mushroom::Appear(float x, float y, int _type)
{
	appearY = y - MUSHROOM_BBOX_HEIGHT - 1;
	this->type = _type;
	this->SetAppearedDirect();
	this->SetPosition(x, y);
	this->SetState(MUSHROOM_STATE_APPEARANCE);
	this->isEnable = true;
}
void Mushroom::Appear(float x, float y)
{
	this->SetPosition(x, y);
	appearY = y - MUSHROOM_BBOX_HEIGHT - 1;
	this->SetAppearedDirect();
	this->SetState(MUSHROOM_STATE_APPEARANCE);
	this->isEnable = true;
	type = MUSHROOM_ANI_POWERUP;
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
void Mushroom::SetAppearedDirect()
{
	PlayScene* scene = (PlayScene*)Game::GetInstance()->GetCurrentScene();
	Mario* mario = scene->GetPlayer();
	this->nx = mario->nx;
}

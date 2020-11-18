#include "Brick.h"
#include <algorithm>

void Brick::Render()
{
	int ani;
	if (state == BRICK_STATE_NORMAL)
		ani = BRICK_ANI_NORMAL;
	else
		ani = BRICK_ANI_EMPTY;
	animation_set->at(ani)->Render(-1,x, y);
	//RenderBoundingBox();
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b,bool isEnable)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
void Brick::SetEmpty()
{
	//if (state != BRICK_STATE_NORMAL)
	//	return;
		this->SetState(BRICK_STATE_EMPTY);
		LPSCENE scence = Game::GetInstance()->GetCurrentScene();
		Mario* mario = ((PlayScene*)scence)->GetPlayer();
		Item* item;
		int form = mario->GetForm();
		switch (type)
		{
		case BRICK_TYPE_ITEM_COIN:
		{
			item = ((PlayScene*)scence)->GetCoin();
			item->Appear(x, y);
			break;
		}
		case BRICK_TYPE_POWERUP:
		{
			if (form == MARIO_SMALL_FORM)
			{
				item = ((PlayScene*)scence)->GetMushroom();
				item->Appear(x,y);
			}
			else 
			{
				item = ((PlayScene*)scence)->GetLeaf();
				item->Appear(x,y);
			}
			break;
		}
		}
}
void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BRICK_STATE_INACTIVE)
		return;
	GameObject::Update(dt, coObjects);
	if(state != BRICK_STATE_NORMAL)
		vy += BRICK_GRAVITY * dt;
	x += dx;
	y += dy;
	if (y > entryY &&  state == BRICK_STATE_EMPTY)
	{
		vy = 0;
		y = entryY;
		state = BRICK_STATE_INACTIVE;
	}
	
}
void Brick::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case BRICK_STATE_NORMAL:
		vx = 0;
		vy = 0;
		break;
	case BRICK_STATE_EMPTY:
		vy = -BRICK_DEFLECT_SPEED;;
		vx = 0;
		break;
	}
}
void InvisibleBrick::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}

void InvisibleBrick::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
#include "Brick.h"
#include <algorithm>
bool Brick::isTransForm = false;
void Brick::Render()
{
	if (isEnable == true )
	{
		int ani;
		if (state == BRICK_STATE_NORMAL)
			ani = BRICK_ANI_NORMAL;
		else 
			ani = BRICK_ANI_EMPTY;
		animation_set->at(ani)->Render(-1, x, y);
	}
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b,bool isEnable)
{
	if (isEnable == true)
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}

}
void Brick::SetEmpty()
{
	if (type != BRICK_TYPE_BREAKABLE)
	{
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
				item->Appear(x, y);
			}
			else
			{
				item = ((PlayScene*)scence)->GetLeaf();
				item->Appear(x, y);
			}
			break;
		}
		case BRICK_TYPE_PSWITCH:
		{
			item = ((PlayScene*)scence)->GetSwitch();
			item->Appear(x, y);
			break;
		}
		}
	}
	else
	{
		this->SetState(BRICK_STATE_BREAK);
	}
}
void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	bool _isTransform = Brick::isTransForm;
	if (state == BRICK_STATE_INACTIVE ||
		isEnable == false)
		return;
	if (GetTickCount() - changeTime >
		BRICK_CHANGE_TIME && _isTransform == true
		&& changeTime != 0)
	{
		DebugOut(L"\nB");
		changeTime = 0;
		this->SetState(BRICK_STATE_NORMAL);
		return;
	}
	else if (_isTransform == true
		&& type == BRICK_TYPE_BREAKABLE && 
		changeTime == 0)
	{
		DebugOut(L"\nA");
		this->SetState(BRICK_STATE_EMPTY);
		changeTime = GetTickCount();
		return;
	}
	else if (type == BRICK_TYPE_BREAKABLE)
		return;
	GameObject::Update(dt, coObjects);
	if(state != BRICK_STATE_NORMAL)
		vy += BRICK_GRAVITY * dt;
	y += dy;
	if (y > entryY && 
		state == BRICK_STATE_EMPTY)
	{
		vy = 0;
		y = entryY;
		this->SetState(BRICK_STATE_INACTIVE);
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
		if (type != BRICK_TYPE_BREAKABLE)
			vy = -BRICK_DEFLECT_SPEED;
		vx = 0;
		break;
	case BRICK_STATE_BREAK:
		vx = 0;
		vy = 0;
		isEnable = false;
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
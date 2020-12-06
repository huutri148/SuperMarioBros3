#include "Brick.h"
#include <algorithm>
bool Brick::isTransForm = false;
void Brick::Render()
{
	int ani;
	if (state == BRICK_STATE_NORMAL)
		ani = BRICK_ANI_NORMAL;
	else 
		ani = BRICK_ANI_EMPTY;
	animation_set->at(ani)->Render(-1, x, y);
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
	if (state == BRICK_STATE_INACTIVE)
		return;
	if (type != BRICK_BREAKABLE_TYPE &&
		state != BRICK_STATE_EMPTY)
	{
		this->SetState(BRICK_STATE_EMPTY);
		isUsed = true;
	}
	else if(type == BRICK_BREAKABLE_TYPE)
	{
		this->SetState(BRICK_STATE_BREAK);
	}
}
void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BRICK_STATE_INACTIVE ||
		isEnable == false)
		return;
	if (type == BRICK_BREAKABLE_TYPE)
	{
		bool _isTransform = Brick::isTransForm;
		if (_isTransform == false)
		{
			this->SetState(BRICK_STATE_NORMAL);
		}
		else
		{
			this->SetState(BRICK_STATE_EMPTY);
		}
		return;
	}
	GameObject::Update(dt, coObjects);
	if(state != BRICK_STATE_NORMAL)
		vy += BRICK_GRAVITY * dt;
	y += dy;
	if (y > entryY  && 
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
		if (type != BRICK_BREAKABLE_TYPE)
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
/// <summary>
/// Với Brick ở trạng thái Breakable thì state break sẽ làm cho 
/// viên gạch biến mất và thêm hiệu ứng
/// ngay cả ở trạng thái là đồng tiền
/// </summary>
void Brick::Used()
{
	this->SetState(BRICK_STATE_BREAK);
}
bool Brick::CanUsed()
{
	if (this->state == BRICK_STATE_EMPTY
		&& Brick::isTransForm == true
		&& type == BRICK_BREAKABLE_TYPE)
		return true;
	return false;
}
bool Brick::Breakable()
{
	if (type == BRICK_BREAKABLE_TYPE)
		return true;
	return false;
}
void InvisibleBrick::Render()
{
	animation_set->at(0)->Render(-1, x, y);
}

void InvisibleBrick::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
void Brick::DropItem(Grid* grid)
{
	LPSCENE scence = Game::GetInstance()->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	Item* item;
	int form = mario->GetForm();
	switch (type)
	{
	case BRICK_ITEM_COIN_TYPE:
	{
		item = new Coin(COIN_TYPE_1);
		item->Appear(entryX, entryY);
		mario->GainMoney(1);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	case BRICK_POWER_UP_TYPE:
	{
		if (form == MARIO_SMALL_FORM)
			item = new Mushroom(MUSHROOM_TYPE_POWERUP);
		else
			item = new RaccoonLeaf();
		item->Appear(x, y);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	case BRICK_PSWITCH_TYPE:
	{
		item = new PSwitch();
		item->Appear(x, y);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	case BRICK_ITEM_EXTRA_LIFE_TYPE:
	{
		item = new Mushroom(MUSHROOM_TYPE_1UP);
		item->Appear(x, y);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	}
	isUsed = false;
}

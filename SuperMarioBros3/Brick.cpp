#include "Brick.h"
#include"Player.h"
#include <algorithm>
bool Brick::isTransForm = false;
void Brick::Render()
{
	int ani;
	if (state == BRICK_STATE_NORMAL)
		ani = BRICK_ANI_NORMAL;
	else 
		ani = BRICK_ANI_EMPTY;
	animation_set->at(ani)->Render(-1, round(x), round(y));
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
}
void Brick::SetEmpty(bool canBreak)
{
	if (state == BRICK_STATE_INACTIVE)
		return;

	if (state != BRICK_STATE_EMPTY && vy == 0)
	{
		if(type != BRICK_BREAKABLE_TYPE && coin == 0)
			this->SetState(BRICK_STATE_EMPTY);
		else
		{
			vy = -BRICK_DEFLECT_SPEED;
		}
	}
		

	if(type == BRICK_BREAKABLE_TYPE && canBreak == true)
	{
		this->Broken();
	}
	Player::GetInstance()->GainPoint(100);
}
void Brick::Broken()
{
	this->SetState(BRICK_STATE_BREAK);
	Game* game = Game::GetInstance();
	PlayScene* scene ;
	if (dynamic_cast<PlayScene*>(game->GetCurrentScene()))
	{
		scene = (PlayScene*)(game->GetCurrentScene());
		Grid* grid = scene->GetGrid();
		if (grid == NULL)
			return;
		BrokenBrickEffect* brokenBrick = new BrokenBrickEffect(x, y);
		Unit* unit = new Unit(grid, brokenBrick, x, y);
	}
}
void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BRICK_STATE_INACTIVE ||
		isEnable == false)
		return;
	if (type == BRICK_BREAKABLE_TYPE && vy ==  0)
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

	if(vy != 0)
		vy += BRICK_GRAVITY * dt;
	y += dy;
	if (y > entryY  && vy != 0)
	{
		vy = 0;
		y = entryY;
		this->DropItem();

		if (coin == 0)
			this->SetState(BRICK_STATE_INACTIVE);
		else
			this->SetState(BRICK_STATE_NORMAL);
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
		if(type != BRICK_BREAKABLE_TYPE)
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
	if (this->state == BRICK_STATE_EMPTY && 
			Brick::isTransForm == true &&
			type == BRICK_BREAKABLE_TYPE)
		return true;
	return false;
}
bool Brick::Breakable()
{
	if (type == BRICK_BREAKABLE_TYPE)
		return true;
	return false;
}
void Brick::DropItem()
{
	LPSCENE scence = Game::GetInstance()->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	Grid* grid = ((PlayScene*)scence)->GetGrid();
	Item* item;
	int form = mario->form;
	switch (type)
	{
	case BRICK_ITEM_COIN_TYPE:
	{
		coin -= 1;
		item = new Coin(COIN_TYPE_1);
		item->Appear(entryX, entryY);
		Player::GetInstance()->GainMoney(1);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	case BRICK_POWER_UP_TYPE:
	{
		coin -= 1;
		if (form == MARIO_SMALL_FORM)
			item = new Mushroom(MUSHROOM_TYPE_POWERUP);
		else
			item = new RaccoonLeaf();
		// Đẩy ra 1 tí khi va chạm gạch bên phải sẽ không bị chồng boundingbox
		item->Appear(x + 1.0f, y);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	case BRICK_PSWITCH_TYPE:
	{
		coin -= 1;
		item = new PSwitch();
		item->Appear(x, y);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	case BRICK_ITEM_EXTRA_LIFE_TYPE:
	{
		coin -= 1;
		item = new Mushroom(MUSHROOM_TYPE_1UP);
		// Đẩy ra 1 tí khi va chạm gạch bên phải sẽ không bị chồng boundingbox
		item->Appear(x + 1.0f, y);
		Unit* unit = new Unit(grid, item, x, y);
		break;
	}
	}
}

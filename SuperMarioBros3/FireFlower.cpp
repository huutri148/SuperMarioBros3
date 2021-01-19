#include "FireFlower.h"
#include"Game.h"
#include"Player.h"
void FireFlower::Render()
{
	int ani = FIREFLOWER_ANI_APPEAR;
	animation_set->at(ani)->Render(-1, round(x), round(y));
}

void FireFlower::GetBoundingBox(float& l, float& t, float& r,
	float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + FIREFLOWER_BBOX_WIDTH;
	b = y + FIREFLOWER_BBOX_HEIGHT;
}
void FireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable == false)
		return;
	GameObject::Update(dt, coObjects);
	if (y <= appearY)
	{
		y = appearY;
		vy = 0;
	}
	y += dy;
}
void FireFlower::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case FIREFLOWER_STATE_APPEAR:
		isEnable = true;
		vx = 0;
		vy = -FIREFLOWER_SPEED_APPEAR_Y;
		break;
	case FIREFLOWER_STATE_INACTIVE:
		isEnable = false;
		vx = 0;
		vy = 0;
		break;
	}
}
void FireFlower::Appear(float x, float y)
{
	this->SetPosition(x, y);
	appearY = y - FIREFLOWER_BBOX_HEIGHT;
	this->SetState(FIREFLOWER_STATE_APPEAR);
}
void FireFlower::Used()
{
	if (state != FIREFLOWER_STATE_INACTIVE)
	{
		LPSCENE scence = Game::GetInstance()->GetCurrentScene();
		Mario* mario = ((PlayScene*)scence)->GetPlayer();
		Grid* grid = ((PlayScene*)scence)->GetGrid();
		this->SetState(FIREFLOWER_STATE_INACTIVE);
		mario->TurnFireForm();
		PointEffect* effect = new PointEffect(x, y, POINT_TYPE_1000);
		Unit* unit = new Unit(grid, effect, x, y);
		Player::GetInstance()->GainPoint(1000);
	}
}

FireFlower::FireFlower()
{
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_FIREFLOWER);
	this->SetAnimationSet(ani_set);
}
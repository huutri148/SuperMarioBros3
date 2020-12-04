#include "PointEffect.h"
PointEffect::PointEffect(float x, float y,int type)
{
	this->type = type;
	this->x = x;
	this->y = y;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(74);
	this->SetAnimationSet(ani_set);
	this->isEnable = true;
	this->SetState(POINT_STATE_APPEAR);
}
void PointEffect::Render()
{
	if (state == POINT_STATE_INACTIVE)
		return;
	int ani = 0;
	switch (type)
	{
	case POINT_TYPE_100:
		ani = POINT_ANI_100;
		break;
	case POINT_TYPE_200:
		ani = POINT_ANI_200;
		break;
	case POINT_TYPE_400:
		ani = POINT_ANI_400;
		break;
	case POINT_TYPE_800:
		ani = POINT_ANI_800;
		break;
	case POINT_TYPE_1000:
		ani = POINT_ANI_1000;
		break;
	case POINT_TYPE_2000:
		ani = POINT_ANI_2000;
		break;
	case POINT_TYPE_4000:
		ani = POINT_ANI_4000;
	case POINT_TYPE_8000:
		ani = POINT_ANI_8000;
	case POINT_TYPE_1UP:
		ani = POINT_ANI_1UP;
		break;
	}
	animation_set->at(ani)->Render(-1, round(x), round(y));
}
void PointEffect::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case POINT_STATE_APPEAR:
		vx = 0;
		vy = -POINT_APPEAR_SPEED;
		appearTime = GetTickCount();
		break;
	case POINT_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}
void PointEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != POINT_STATE_APPEAR)
		return;
	if (GetTickCount() - appearTime > POINT_APPEAR_TIME
		&& appearTime != 0)
		this->SetState(POINT_STATE_INACTIVE);
	GameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}


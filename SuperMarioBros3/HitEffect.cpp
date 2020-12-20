#include "HitEffect.h"
HitEffect::HitEffect(float x, float y)
{
	this->x = x;
	this->y = y;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_HIT_EFFECT);
	this->SetAnimationSet(ani_set);
	this->isEnable = true;
	this->SetState(HITEFFECT_STATE_APPEAR);
}
void HitEffect::Render()
{
	if (state != HITEFFECT_STATE_APPEAR)
		return;
	animation_set->at(0)->Render(-1, round(x), round(y));
}

void HitEffect::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case HITEFFECT_STATE_APPEAR:
		vx = 0;
		vy = 0;
		appearTime = GetTickCount();
		break;
	case HITEFFECT_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}
void HitEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != HITEFFECT_STATE_APPEAR)
		return;
	if (GetTickCount() - appearTime > HITEFFECT_APPEAR_TIME
		&& appearTime != 0)
		this->SetState(POINT_STATE_INACTIVE);
}

#pragma once
#include"Enemy.h"
class BoomerangBrother : public Enemy
{
	float lastStoping;
	DWORD changeStateTime;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleTimeSwitchState();


public:
	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetBeingStromped();
	virtual void SetBeingSkilled(int nx);
	virtual void Inactive() { this->SetState(BOOMERANG_BROTHER_STATE_INACTIVE); }
	virtual void Active() { this->SetState(BOOMERANG_BROTHER_STATE_WALKING); };
	virtual bool IsInactive()
	{
		if (this->state == BOOMERANG_BROTHER_STATE_INACTIVE)
			return true;
		return false;
	}
	BoomerangBrother(float x, float y) :Enemy(x, y)
	{
		this->SetState(BOOMERANG_BROTHER_STATE_WALKING);
		vx = -BOOMERANG_BROTHER_SPEED_X;
		lastStoping = x;
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_BOOMERANG_BROTHER);
		this->SetAnimationSet(ani_set);
	}
	BoomerangBrother() :Enemy()
	{
		this->SetState(BOOMERANG_BROTHER_STATE_WALKING);
		vx = -BOOMERANG_BROTHER_SPEED_X;
		this->isEnable = true;
		lastStoping = x;
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_BOOMERANG_BROTHER);
		this->SetAnimationSet(ani_set);
	}
};


#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Define.h"

class Goomba : public Enemy
{
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom,bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleTimeSwitchState() ;
	

public:
	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetBeingStomped();
	virtual void SetBeingSkilled(int nx);
	virtual void Inactive() { this->SetState(GOOMBA_STATE_INACTIVE); }
	virtual void Active() { this->SetState(GOOMBA_STATE_WALKING); };
	virtual bool IsInactive()
	{
		if (this->state == GOOMBA_STATE_INACTIVE)
			return true;
		return false;
	}
	Goomba(float x, float y) :Enemy(x,y)
	{
		this->SetState(GOOMBA_STATE_WALKING);
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_BROWN_GOOMBA);
		this->SetAnimationSet(ani_set);	
	}
	Goomba() :Enemy()
	{
		this->SetState(GOOMBA_STATE_WALKING);
		this->isEnable = true;
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_RED_GOOMBA);
		this->SetAnimationSet(ani_set);
	}
};
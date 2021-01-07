#pragma once
#include"Enemy.h"
#include"Boomerang.h"
class BoomerangBrother : public Enemy
{
	float lastStoping;
	DWORD changeStateTime;
	unsigned int indexBoomerang = 0;
	vector<Boomerang*> weapon;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleTimeSwitchState();


public:
	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetBeingStomped();
	virtual void SetBeingSkilled(int nx);
	virtual int GetWidth() { return BOOMERANG_BROTHER_BBOX_WIDTH; };
	virtual void Inactive() { this->SetState(BOOMERANG_BROTHER_STATE_INACTIVE); }
	virtual void Active() { this->SetState(BOOMERANG_BROTHER_STATE_WALKING); };
	virtual bool IsInactive()
	{
		if (this->state == BOOMERANG_BROTHER_STATE_INACTIVE)
			return true;
		return false;
	}

	void ThrowingBoomerang();


	BoomerangBrother(float x, float y) :Enemy(x, y)
	{
		this->SetState(BOOMERANG_BROTHER_STATE_WALKING);
		vx = -BOOMERANG_BROTHER_SPEED_X;
		lastStoping = x;
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_BOOMERANG_BROTHER);
		this->SetAnimationSet(ani_set);

		for (unsigned int i = 0; i < 2; i++)
		{
			Boomerang* boom = new Boomerang();
			weapon.push_back(boom);
		}
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


		weapon.push_back(new Boomerang());
		weapon.push_back(new Boomerang());
	}
};


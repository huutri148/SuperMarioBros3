#pragma once
#include"Enemy.h"
#include"Goomba.h"
#include"Grid.h"
class ParaGoomba :public Enemy
{
	DWORD switchStateTime;
	bool isInGround;
	int jumpingCount = 0;
	Goomba* goomba;
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
	virtual bool IsInactive();
	virtual void Active();
	virtual void Inactive();
	void ChangeToGoomba(Grid* grid);
	void Chasing();
	virtual int GetWidth() { return PARAGOOMBA_BBOX_WIDTH; };
	ParaGoomba(float x, float y) :Enemy(x, y)
	{
		this->SetState(PARAGOOMBA_STATE_WALKING);
		goomba = new Goomba();
		goomba->isEnable = false;
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_PARA_GOOMBA);
		this->SetAnimationSet(ani_set);
	}
	ParaGoomba() :Enemy()
	{
		this->SetState(PARAGOOMBA_STATE_WALKING);
	}
};


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
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT>& coEvents);
	virtual void HandleTimeSwitchState() ;
	

public:
	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetBeingStromped();
	virtual void SetBeingSkilled();
	virtual void EnableAgain();
	Goomba(float x, float y) :Enemy(x,y)
	{
		this->SetState(GOOMBA_STATE_WALKING);
	}
	Goomba() :Enemy()
	{
		this->SetState(GOOMBA_STATE_WALKING);
	}
};
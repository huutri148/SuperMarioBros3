#pragma once
#include"Enemy.h"
#include"Goomba.h"
class ParaGoomba :public Enemy
{
	DWORD switchStateTime;
	/*Goomba* goomba;*/
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
	ParaGoomba(float x, float y) :Enemy(x, y)
	{
		this->SetState(PARAGOOMBA_STATE_WALKING);
	}
	ParaGoomba() :Enemy()
	{
		this->SetState(PARAGOOMBA_STATE_WALKING);
	}
};


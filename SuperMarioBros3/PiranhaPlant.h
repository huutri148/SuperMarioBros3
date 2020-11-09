#pragma once
#include "Enemy.h"
#include"Define.h"
class PiranhaPlant :public Enemy
{
	DWORD switchTime;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	virtual bool IsDead();
	virtual void SetBeingStromped() = 0;
	virtual void SetBeingSkilled();
	virtual void EnableAgain();
	PiranhaPlant(float x, float y) :Enemy(x, y)
	{
		this->SetState(PIRIHANPLANT_STATE_DARTING);
	}
	PiranhaPlant() :Enemy()
	{
		this->SetState(PIRIHANPLANT_STATE_DARTING);
	}

};


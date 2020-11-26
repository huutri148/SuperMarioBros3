#pragma once
#include "Enemy.h"
#include"Define.h"
class PiranhaPlant :public Enemy
{
	DWORD switchTime;
	bool isOutOfPipe;
	int type;
	virtual void HandleTimeSwitchState();
public:
	virtual void GetBoundingBox(float& left, float& top, float& right,
		float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	virtual bool IsDead();
	virtual void SetBeingStromped() ;
	virtual void SetBeingSkilled(int nx);
	virtual void Inactive() { this->SetState(PIRANHAPLANT_STATE_INACTIVE); }

	PiranhaPlant(float x, float y, int _type = PIRANHAPLANT_GREEN_TYPE);
	
};


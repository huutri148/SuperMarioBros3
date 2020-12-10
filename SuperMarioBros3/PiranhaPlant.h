#pragma once
#include "Enemy.h"
#include"Define.h"
class PiranhaPlant :public Enemy
{
	DWORD switchTime;
	bool isOutOfPipe;
	float lastStateY;
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
	virtual void Active() { this->SetState(PIRANHAPLANT_STATE_DARTING); };
	virtual bool IsInactive()
	{
		if (this->state ==PIRANHAPLANT_STATE_INACTIVE)
			return true;
		return false;
	}

	PiranhaPlant(float x, float y, int _type = PIRANHAPLANT_GREEN_TYPE);
	
};


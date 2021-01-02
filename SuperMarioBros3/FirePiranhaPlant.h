#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Mario.h"
#include"Grid.h"
class FirePiranhaPlant :public Enemy
{
	DWORD switchTime;
	bool isOutOfPipe;
	int type;
	bool isShooted = false;
	bool shootInGround;
	float lastStateY;
	float limit;
	virtual void HandleTimeSwitchState();

	void GetDirect();
public: 
	bool canShoot = false;
	virtual void GetBoundingBox(float& left, float& top, float& right,
		float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void Shooting();
	virtual bool IsDead();
	virtual void SetBeingStomped();
	virtual void SetBeingSkilled(int nx);
	virtual void Inactive() { this->SetState(FIREPIRANHAPLANT_STATE_INACTIVE); };
	virtual void Active() { this->SetState(FIREPIRANHAPLANT_STATE_DARTING); };
	virtual bool IsInactive()
	{
		if (this->state == FIREPIRANHAPLANT_STATE_INACTIVE)
			return true;
		return false;
	}
	FirePiranhaPlant(float x, float y, float limit , int _type = PIRANHAPLANT_RED_TYPE);
};


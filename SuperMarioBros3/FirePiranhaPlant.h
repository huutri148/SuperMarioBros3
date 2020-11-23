#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Mario.h"
class FirePiranhaPlant :public Enemy
{
	DWORD switchTime;
	bool isOutOfPipe;
	int type;
	bool isShooted = false;
	bool shootInGround;
	virtual void HandleTimeSwitchState();
	void Shooting();
	void GetDirect();
public: 
	virtual void GetBoundingBox(float& left, float& top, float& right,
		float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	virtual bool IsDead();
	virtual void SetBeingStromped();
	virtual void SetBeingSkilled(int nx);
	virtual void EnableAgain();
	FirePiranhaPlant(float x, float y, int _type = PIRANHAPLANT_RED_TYPE);
};


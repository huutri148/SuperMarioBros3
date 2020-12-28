#pragma once
#include"Enemy.h"
#include"Grid.h"
class RedKoopaParaTroopa : public Enemy
{
	Enemy* kooPa;
	DWORD changeDirectTime;
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
	virtual bool IsInactive();
	virtual void Active();
	virtual void Inactive();
	void ChangeToKoopa(Grid* grid);

	RedKoopaParaTroopa(float x, float y, float limit = 80.0f);
	RedKoopaParaTroopa();
};


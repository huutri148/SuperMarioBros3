#pragma once
#include"KoopaTroopa.h"

class KoopaParaTroopa :public Enemy
{
	//float lastStandingY;
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

	KoopaParaTroopa(float x, float y) :Enemy(x, y)
	{
		this->SetState(PARATROOPA_STATE_JUMPING);
	}
	KoopaParaTroopa() :Enemy()
	{
		this->SetState(PARATROOPA_STATE_JUMPING);
	}
};


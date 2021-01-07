#pragma once
#include"Enemy.h"
#include"Grid.h"
#include"KoopaTroopa.h"

class KoopaParaTroopa :public Enemy
{
	// It's a lame way !!!
	// Todo: find a way to specify KoopaTroopa instead of Enemy
	Enemy* kooPa;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int GetWidth() { return PARATROOPA_BBOX_WIDTH; };
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
	void ChangeToKoopa(Grid* grid);

	KoopaParaTroopa(float x, float y);
	KoopaParaTroopa();
};


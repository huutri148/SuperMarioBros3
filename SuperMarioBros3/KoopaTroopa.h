#pragma once
#include"GameObject.h"
#include"Enemy.h"
#include "Mario.h"
#include"Define.h"
#include"Utils.h"
#include "Brick.h"
#include "Ground.h"
#include "Pipe.h"
#include "Block.h"
#include"Game.h"


class KoopaTroopa :public Enemy
{
	int type;

	DWORD hidingTime = 0;
	DWORD turnWalkingTime = 0;
	
public:
	//chuyển thành hàm
	bool isPickedUp;
	bool isBumped;
	bool forceShell = false;
	
	float lastStanding_Y;
	bool CanPullBack = false;
	void IsKicked(int nx);
	bool IsHiding();
	virtual bool IsDead();
	void PickUpBy();
	
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleTimeSwitchState();
	

	virtual void SetState(int state);
	virtual void SetBeingStomped();
	virtual void SetBeingSkilled(int nx);
	virtual void Inactive() { this->SetState(KOOPATROOPA_STATE_INACTIVE); }
	virtual void Active() { this->SetState(KOOPATROOPA_STATE_WALKING); };
	virtual bool IsInactive()
	{
		if (this->state == KOOPATROOPA_STATE_INACTIVE)
			return true;
		return false;
	}
	KoopaTroopa(float x, float y, int _type = KOOPATROOPA_GREEN_TYPE);
	KoopaTroopa(int type =KOOPATROOPA_GREEN_TYPE);
};


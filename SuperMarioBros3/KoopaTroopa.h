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
	//Mario* mario;
	int type;
	bool isBumped;
	DWORD hidingTime = 0;
	DWORD turnWalkingTime = 0;
	
public:

	//chuyển thành hàm
	bool isPickedUp;
		
	
	void IsKicked(int nx);
	bool IsHiding();
	virtual bool IsDead();
	void PickUpBy(Mario* mario);
	
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void HandleTimeSwitchState();
	

	virtual void SetState(int state);
	virtual void SetBeingStromped();
	virtual void SetBeingSkilled();
	virtual void EnableAgain();
	KoopaTroopa(float x, float y, int _type = KOOPATROOPA_GREEN_TYPE);
};


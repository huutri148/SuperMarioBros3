#pragma once
#include"GameObject.h"
#include"Enemy.h"
#include "Mario.h"
#include"Define.h"


class KoopaTroopa :public Enemy
{
	Mario* mario;
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
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	

	virtual void SetState(int state);
	//virtual void SetDie(bool n);
	virtual void SetBeingStromped();
	virtual void SetBeingSkilled();
	virtual void EnableAgain();
	KoopaTroopa(int x, int y, int _type = 0);
};


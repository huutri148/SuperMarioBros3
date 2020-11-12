#pragma once
#include"GameObject.h"
#include"Define.h"
class FirePlantBullet :public GameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual void SetState(int state);
	void Shoot(int _x, int _y, int _nx,int _direct)
	{
		this->x = _x;
		this->y = _y;
		this->isEnable = true;
		this->nx = _nx;
		this->SetState(_direct);
	}
	FirePlantBullet()
	{
		this->isEnable = false;
	}
};


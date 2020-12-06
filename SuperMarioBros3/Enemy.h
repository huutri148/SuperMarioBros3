#pragma once
#include"GameObject.h"

class Enemy :public GameObject
{	
protected:
	DWORD deathTime = 0;
	float entryX, entryY;
	D3DXVECTOR2 playerPosition;
public:
	bool isAbleToActive = false;
	bool isDead = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;


	virtual bool IsEnable() { return this->isEnable; };
	virtual bool IsDead() = 0;
	virtual bool IsInactive() = 0;
	

	virtual void HandleTimeSwitchState() = 0;
	virtual void ChangeDirect() { this->nx = -nx; this->vx = -this->vx; };
	virtual void Inactive() = 0;
	virtual void Active() = 0;
	void AbleToActive();

	virtual void SetEntryPosition(float x, float y);
	void GetEntryPosition(float& ex, float& ey) { ex = entryX; ey = entryY; };
	virtual void SetState(int state);

	void SetEnable(bool _enable) { this->isEnable = _enable; };
	void SetDead() { this->isDead = true; };
	virtual void SetBeingStromped() = 0;
	virtual void SetBeingSkilled(int nx) = 0;
	void GetPlayerPosition(float x, float y)
	{ 
		playerPosition.x = x;
		playerPosition.y = y;
	}

	Enemy();
	Enemy(float x, float y) { this->x = x;
	this->y = y; this->SetEntryPosition(x, y); isEnable = true; }
	~Enemy();
};


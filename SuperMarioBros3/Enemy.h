#pragma once
#include"GameObject.h"

class Enemy :public GameObject
{	
protected:
	DWORD deathTime;
	float entryX, entryY;
	
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	virtual void SetState(int state);
	virtual bool IsDead() = 0;
	virtual void SetBeingStromped() = 0;
	virtual void SetBeingSkilled() = 0;
	virtual void HandleTimeSwitchState() = 0;
	
	virtual void SetEntryPosition(float x, float y);
	void SetEnable() { this->isEnable = true; };
	virtual bool IsEnable() { return this->isEnable; };
	void EnableAgain();
	bool IsAbleToActive();

	Enemy();
	Enemy(float x, float y) { this->x = x; this->y = y; this->SetEntryPosition(x, y); isEnable = false; }
	~Enemy();
};


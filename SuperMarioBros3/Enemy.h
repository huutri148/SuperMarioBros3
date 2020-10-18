#pragma once
#include"GameObject.h"
class Enemy :public CGameObject
{	
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	virtual void SetState(int state);
	virtual bool IsDead() = 0;
	virtual	void SetDie() = 0;
	Enemy();
	~Enemy();
};


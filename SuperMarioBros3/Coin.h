#pragma once
#include"GameObject.h"
#include"Define.h"
class Coin : public GameObject
{
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
	//	vector<LPCOLLISIONEVENT>& coEvents);
public:
	virtual void SetState(int state);
	Coin() { this->SetState(COIN_STATE_ACTIVE); };
	void Disappearance();
};


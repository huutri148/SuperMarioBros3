#pragma once
#include"GameObject.h"
#include"Define.h"
#include"Item.h"
class Coin : public Item
{
	int type;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
	//	vector<LPCOLLISIONEVENT>& coEvents);
public:
	void Appear(float x, float y);
	void Used();
	int GetType() { return this->type; }
	virtual void SetState(int state);
	Coin(int type);
};


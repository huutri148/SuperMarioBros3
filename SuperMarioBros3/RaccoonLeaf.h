#pragma once
#include"GameObject.h"
#include"Define.h"
#include"Utils.h"
#include"Item.h"
//#include"Game.h"

class RaccoonLeaf :public Item
{
	DWORD changeDirectTime = 0;
public:
	virtual void Render();
	RaccoonLeaf();
	RaccoonLeaf(float x, float y) { 
		this->x = x; 
		this->y = y;
		this->SetState(RACCOONLEAF_STATE_APPEARANCE);
		this->isEnable = false;
	}
	virtual void Appear(float x, float y);
	virtual void Update(DWORD dt, 
		vector<LPGAMEOBJECT>* coObjects);
	void SetState(int _state);
	virtual void GetBoundingBox(float& l, float& t, float& r,
		float& b, bool isEnable);
	virtual void Used();
};


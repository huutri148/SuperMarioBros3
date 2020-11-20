#pragma once
#include"Item.h"
class PSwitch :public Item
{
	DWORD appearTime = 0;
public:
	virtual void Render();
	PSwitch();
	virtual void Appear(float x, float y);
	virtual void Update(DWORD dt,
		vector<LPGAMEOBJECT>* coObjects);
	void SetState(int _state);
	virtual void GetBoundingBox(float& l, float& t, float& r,
		float& b, bool isEnable);
	virtual void Used();
};


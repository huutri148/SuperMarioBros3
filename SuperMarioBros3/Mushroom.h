#pragma once
#include "GameObject.h"
#include "Item.h"
class Mushroom :public Item
{
public:
	virtual void Render();
	void Appear(float x, float y);
	virtual void Update(DWORD dt,
		vector<LPGAMEOBJECT>* coObjects);
	void SetState(int _state);
	virtual void GetBoundingBox(float& l, float& t, float& r,
		float& b, bool isEnable);
	void Used();
	Mushroom();
};


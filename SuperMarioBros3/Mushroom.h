#pragma once
#include "GameObject.h"
#include "Item.h"
class Mushroom :public Item
{
	int type;
	float appearY;
public:
	virtual void Render();
	void Appear(float x, float y, int _type = MUSHROOM_ANI_POWERUP);
	void Appear(float x, float y);
	virtual void Update(DWORD dt,
		vector<LPGAMEOBJECT>* coObjects);
	void SetState(int _state);
	void SetAppearedDirect();
	virtual void GetBoundingBox(float& l, float& t, float& r,
		float& b, bool isEnable);
	void Used();
	Mushroom();
};


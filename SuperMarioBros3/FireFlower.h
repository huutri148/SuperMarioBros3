#pragma once
#include"Item.h"
class FireFlower:public Item
{
	float appearY;
public:
	virtual void Render();
	FireFlower();
	virtual void Appear(float x, float y);
	virtual void Update(DWORD dt,
		vector<LPGAMEOBJECT>* coObjects);
	void SetState(int _state);
	virtual void GetBoundingBox(float& l, float& t, float& r,
		float& b, bool isEnable);
	virtual void Used();
	virtual int GetWidth() { return FIREFLOWER_BBOX_WIDTH; };

};


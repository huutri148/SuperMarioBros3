#pragma once
#include"GameObject.h"
class Item :public GameObject
{
public:
	virtual void Appear(float x, float y) = 0;
	virtual void Used() = 0;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Render() = 0;
};


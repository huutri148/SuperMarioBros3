#pragma once
#include"GameObject.h"
class PointEffect :public GameObject
{
	int type;
	DWORD appearTime = 0;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	PointEffect(float x, float y,int type = POINT_TYPE_100);
	virtual void SetState(int state);
};


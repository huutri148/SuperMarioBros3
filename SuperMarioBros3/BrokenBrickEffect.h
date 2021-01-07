#pragma once
#include"GameObject.h"
class BrokenBrickEffect :public GameObject
{
	vector<LPSPRITE> brokenBrickSprite;
	DWORD appearTime;
	float x1, x2;
	float y1, y2;
	float vy1, vy2;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	BrokenBrickEffect(float x, float y);
	virtual void SetState(int state);
	virtual int GetWidth() { return 0; };
};


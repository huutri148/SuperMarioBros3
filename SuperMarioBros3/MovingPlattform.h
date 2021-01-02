#pragma once
#include"GameObject.h"
class MovingPlattform : public GameObject
{
	float entryX, entryY;

public:
	bool isAbleToActive = true;
	bool isBeingTouched = false;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	MovingPlattform(float entryX, float entryY);
	void AbleToActive();
};


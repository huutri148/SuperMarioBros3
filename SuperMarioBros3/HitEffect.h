#pragma once
#include"GameObject.h"
class HitEffect : public GameObject
{
	DWORD appearTime = 0;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual int GetWidth() { return 0; };
public:
	HitEffect(float x, float y);
	virtual void SetState(int state);
};


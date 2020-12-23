#pragma once
#include"GameObject.h"
class Portal :public GameObject
{
	DWORD changeTime = 0;
	int idCard;
	LPSPRITE card;
	LPSPRITE congratulate;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	Portal();
	void GetPortal();
	virtual void SetState(int state);
	int GetCardId() { return idCard; };

};


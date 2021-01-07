#pragma once
#include "GameObject.h"
#include "Define.h"
#include "Mario.h"
#include"Game.h"
#include"RaccoonLeaf.h"
#include"Grid.h"
#include"BrokenBrickEffect.h"

class Brick : public GameObject
{
	
	DWORD changeTime;
	int coin;
	
	float entryX, entryY;
	int type;
public:
	static bool isTransForm;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	/*virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT>& coEvents);*/
	virtual void Render();
	Brick(float x, float y, int _type  = BRICK_ITEM_COIN_TYPE)
		:GameObject(x, y) {
		this->entryX = x;
		this->entryY = y;
		type = _type;
		coin = 1;
		this->SetState(BRICK_STATE_NORMAL);
	};

	Brick(float x, float y, int _type , int ncoin )
		:GameObject(x, y) {
		this->entryX = x;
		this->entryY = y;
		type = _type;
		coin = ncoin;
		this->SetState(BRICK_STATE_NORMAL);
	};

	bool Breakable();
	bool CanUsed();
	void Broken();
	void DropItem();
	void Used();
	void SetState(int _state);
	void SetEmpty(bool canBreak);
	virtual int GetWidth() { return BRICK_BBOX_WIDTH; };
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b,bool isEnable);
};




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
	
	float entryX, entryY;
	int type;
public:
	static bool isTransForm;
	bool isUsed = false;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	/*virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT>& coEvents);*/
	virtual void Render();
	Brick(float x, float y, int _type  = BRICK_ITEM_COIN_TYPE)
		:GameObject(x, y) {
		this->entryX = x;
		this->entryY = y;
		type = _type;
		this->SetState(BRICK_STATE_NORMAL);
	};
	bool Breakable();
	bool CanUsed();
	void Broken();
	void DropItem();
	void Used();
	void SetState(int _state);
	void SetEmpty();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b,bool isEnable);
};



class InvisibleBrick : public GameObject
{
private:
	int type;
public:
	InvisibleBrick(int _type = 0) { type = _type; };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	int GetType() { return type; };
};
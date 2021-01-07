#pragma once
#include"GameObject.h"
#include "Grid.h"
class RaccoonTail :public GameObject
{
	Unit* unit;
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	virtual void SetState(int state);
	void Attack(float x, float y,int nx, Grid* grid) {
		this->x = x;
		this->y = y;
		this->nx = nx;
		this->isEnable = true;
		if (unit == NULL)
		{
			unit = new Unit(grid, this, x, y);
		}
		else
			unit->Move(x, y);
		this->SetState(RACCOONTAIL_STATE_ATTACK);
	};
	virtual int GetWidth() { return RACCOONTAIL_BBOX_WIDTH; };
};


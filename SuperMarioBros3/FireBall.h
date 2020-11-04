#pragma once
#include"GameObject.h"


#define FIREBALL_GRAVITY 0.008f
#define FIREBALL_SPEED_NX	0.15f
#define FIREBALL_SPEED_NY 0.05f
#define FIREBALL_BBOX_HEIGHT 16
#define FIREBALL_BBOX_WIDTH 8

#define FIREBALL_STATE_NORMAL	100
#define FIREBALL_STATE_EXPLODE		200
#define FIREBALL_STATE_INACTIVE	300

#define FIREBALL_ANI_NORMAL		0
#define FIREBALL_ANI_EXPLODE	1


#define FIREBALL_INACTIVE_TIME	200
class FireBall : public GameObject
{
	DWORD explodeTime;
public: 
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual void SetState(int state);
	FireBall(int _x, int _y, int _nx) 
	{
		this->x = _x;
		this->y = _y;
		this->isEnable = true;
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(60);
		this->SetAnimationSet(ani_set);
		this->nx = _nx;
		this->SetState(FIREBALL_STATE_NORMAL);
	}
};
typedef FireBall* LPFireBall;


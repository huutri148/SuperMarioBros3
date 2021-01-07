#pragma once
#include"GameObject.h"
#include"Define.h"
class FirePlantBullet :public GameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual void SetState(int state);
	void Shoot( float x,float y,int _nx,int _direct)
	{
		this->x = x;
		this->y = y;
		this->nx = _nx;
		this->SetState(_direct);
	}
	FirePlantBullet()
	{
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(60);
		this->SetAnimationSet(ani_set);
	}
	virtual int GetWidth() { return FIREBALL_BBOX_WIDTH; };
};


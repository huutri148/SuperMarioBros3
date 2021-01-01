#pragma once
#include"GameObject.h"
#include"Grid.h"
class Boomerang : public GameObject
{
	float startX, startY;
	Unit* unit;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual void SetState(int state);
	void Throw(float x, float y,int nx,Grid* grid);
	Boomerang()
	{
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_BOOMERANG);
		this->SetAnimationSet(ani_set);
		this->SetState(BOOMERANG_STATE_INACTIVE);
	}
};


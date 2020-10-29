#pragma once
#include"GameObject.h"
#include"Enemy.h"
#include "Mario.h"


#define KOOPATROOPA_WALKING_SPEED 0.05f
#define KOOPATROOPA_GRAVITY 0.002f
#define KOOPATROOPA_BUMP_SPEED	0.3f
#define KOOPATROOPA_DIE_DEFLECT_SPEED 0.5f

#define KOOPATROOPA_BBOX_WIDTH 16
#define KOOPATROOPA_BBOX_HEIGHT 26
#define KOOPATROOPA_BBOX_HEIGHT_HIDING 16


#define KOOPATROOPA_STATE_WALKING 100
#define KOOPATROOPA_STATE_HIDING 200
#define KOOPATROOPA_STATE_IS_PICKED_UP 300
#define KOOPATROOPA_STATE_DIE_NX 400



#define KOOPATROOPA_ANI_WALKING 0
#define KOOPATROOPA_ANI_HIDING 1
class KoopaTroopa :public Enemy
{

	Mario* mario;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	bool isPickedUp;
	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetDie(bool n);
	void PickUpBy(Mario* mario) {
		isPickedUp = true;
		this->mario = mario;
		/*this->SetState (KOOPATROOPA_STATE_IS_PICKED_UP);*/
	}
	KoopaTroopa() : Enemy()
	{
		isPickedUp = false;
		isEnable = true;
	}
	void IsKicked(int nx);
	bool IsHiding();
};


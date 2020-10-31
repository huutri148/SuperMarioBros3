#pragma once
#include "Mario.h"
#include "Enemy.h"
#define REDKOOPA_WALKING_SPEED 0.05f
#define REDKOOPA_GRAVITY 0.002f
#define REDKOOPA_BUMP_SPEED	0.3f
#define REDKOOPA_DIE_DEFLECT_SPEED 0.5f

#define REDKOOPA_BBOX_WIDTH 16
#define REDKOOPA_BBOX_HEIGHT 26
#define REDKOOPA_BBOX_HEIGHT_HIDING 16


#define REDKOOPA_STATE_WALKING 100
#define REDKOOPA_STATE_HIDING 200
#define REDKOOPA_STATE_IS_PICKED_UP 300
#define REDKOOPA_STATE_DIE_NX 400



#define REDKOOPA_ANI_WALKING 0
#define REDKOOPA_ANI_HIDING 1
class RedKoopa :public Enemy
{
	Mario* mario;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable);
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
	RedKoopa() : Enemy()
	{
		isPickedUp = false;
		isEnable = true;
	}
	void IsKicked(int nx);
	bool IsHiding();
};


#pragma once
#include"GameObject.h"
#include"Enemy.h"
#include "Mario.h"


#define KOOPATROOPA_TYPE_RED	0
#define KOOPATROOPA_TYPE_GREEN	1

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
#define KOOPATROOPA_STATE_IS_BUMPED 500



#define KOOPATROOPA_ANI_WALKING 0
#define KOOPATROOPA_ANI_HIDING 1
#define KOOPATROOPA_ANI_BUMPING 2
#define KOOPATROOPA_ANI_DEATH	3
class KoopaTroopa :public Enemy
{

	Mario* mario;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int type;
public:
	bool isPickedUp;
	bool isBumped;
	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetDie(bool n);
	void PickUpBy(Mario* mario) {
		isPickedUp = true;
		this->mario = mario;
		/*this->SetState (KOOPATROOPA_STATE_IS_PICKED_UP);*/
	}
	KoopaTroopa(int x, int y, int _type = 0) : Enemy(x,y)
	{
		isPickedUp = false;
		isEnable = true;
		isBumped = false;
		type = _type;
		this->SetState(KOOPATROOPA_STATE_WALKING);
	}
	void IsKicked(int nx);
	bool IsHiding();
};


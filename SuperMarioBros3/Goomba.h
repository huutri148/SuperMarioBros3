#pragma once
#include "GameObject.h"
#include "Enemy.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_GRAVITY		0.002f;
#define GOOMBA_DIE_DEFLECT_SPEED 0.5f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_NX 300
#define GOOMBA_STATE_DIE_NY 400
#define GOOMBA_STATE_INACTIVE 500

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1


#define GOOMA_INACTIVE_TIME	500
class CGoomba : public Enemy
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	

public:
	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetDie(bool n);

};
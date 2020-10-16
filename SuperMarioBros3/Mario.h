#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT		3
#define MARIO_ANI_FIRE_IDLE_RIGHT		4
#define MARIO_ANI_FIRE_IDLE_LEFT		5
#define MARIO_ANI_RACCOON_IDLE_RIGHT	6
#define MARIO_ANI_RACCOON_IDLE_LEFT		7


#define MARIO_ANI_BIG_WALKING_RIGHT			8
#define MARIO_ANI_BIG_WALKING_LEFT			9
#define MARIO_ANI_SMALL_WALKING_RIGHT		10
#define MARIO_ANI_SMALL_WALKING_LEFT		11
#define MARIO_ANI_FIRE_WALKING_RIGHT		12
#define MARIO_ANI_FIRE_WALKING_LEFT			13
#define MARIO_ANI_RACCOON_WALKING_RIGHT		14
#define MARIO_ANI_RACCOON_WALKING_LEFT		15


#define MARIO_ANI_DIE				16

#define	MARIO_SMALL_FORM	1
#define	MARIO_BIG_FORM		2
#define MARIO_FIRE_FORM		3
#define MARIO_RACCOON_FORM	4


#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_FIRE_BBOX_WIDTH 15
#define MARIO_FIRE_BBOX_HEIGHT 27

#define MARIO_RACCOON_BBOX_WIDTH 21
#define MARIO_RACCOON_BBOX_HEIGHT 28

#define MARIO_UNTOUCHABLE_TIME 4000


class CMario : public CGameObject
{
	int form;
	int untouchable;
	DWORD untouchable_start;
public:
	CMario() : CGameObject()
	{
		form = MARIO_RACCOON_FORM;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { form = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void UpForm();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
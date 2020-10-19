#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_LONG_JUMP_SPEED_Y 0.6f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_BRAKE_DEFLECT_SPEED 0.05f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define BUFF_SPEED		0.05f
#define POWER_METER_FULL 7
#define STACK_TIME 3000

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_LONG_JUMP		500
#define MARIO_STATE_RUNNING_RIGHT	600
#define MARIO_STATE_RUNNING_LEFT	700
#define MARIO_STATE_BRAKE_RIGHT		800
#define MARIO_STATE_BRAKE_LEFT		900

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

#define MARIO_ANI_BIG_RUNNING_RIGHT			16
#define MARIO_ANI_BIG_RUNNING_LEFT			17
#define MARIO_ANI_SMALL_RUNNING_RIGHT		18
#define MARIO_ANI_SMALL_RUNNING_LEFT		19
#define MARIO_ANI_FIRE_RUNNING_RIGHT		20
#define MARIO_ANI_FIRE_RUNNING_LEFT			21
#define MARIO_ANI_RACCOON_RUNNING_RIGHT		22
#define MARIO_ANI_RACCOON_RUNNING_LEFT		23
//phanh 
#define MARIO_ANI_BIG_BRAKE_RIGHT			24
#define MARIO_ANI_BIG_BRAKE_LEFT			25
#define MARIO_ANI_SMALL_BRAKE_RIGHT			26
#define MARIO_ANI_SMALL_BRAKE_LEFT			27
#define MARIO_ANI_FIRE_BRAKE_RIGHT			28
#define MARIO_ANI_FIRE_BRAKE_LEFT			29
#define MARIO_ANI_RACCOON_BRAKE_RIGHT		30
#define MARIO_ANI_RACCOON_BRAKE_LEFT		31



#define MARIO_ANI_DIE						32

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

#define MARIO_UNTOUCHABLE_TIME 1000
#define MARIO_LONG_JUMP_TIME 200

class CMario : public CGameObject
{
	int form;
	int untouchable;
	DWORD untouchable_start;
	DWORD stack_time_start;
	int power_melter_stack;
	int jump_stack;
	DWORD long_jump;
	bool isInGround;
public:
	bool isJump;
	CMario() : CGameObject()
	{
		form = MARIO_BIG_FORM;
		untouchable = 0;
		isJump = false;
		power_melter_stack = 1;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { form = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartJumping() { long_jump = GetTickCount(); isInGround = false; isJump = true; }
	void UpForm();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Jump();
	void unJump();
	void FillUpPowerMelter();
	void LosePowerMelter(); 
	void Information();
};
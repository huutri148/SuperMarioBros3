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
#define BUFF_SPEED		0.02f	// tốc độ tăng lên khi tích stack
#define POWER_METER_FULL 7		// số stack tối đa#define STACK_TIME 500			//thời gian để tích 1 stack
#define STACK_TIME 200			//thời gian để tích 1 stack

#define MARIO_STATE_IDLE	0
#define MARIO_STATE_WALKING	100
#define MARIO_STATE_RUNNING	200
#define MARIO_STATE_JUMPING	300
#define MARIO_STATE_LONG_JUMPING	400
#define MARIO_STATE_BRAKING	500
#define MARIO_STATE_KICK	600
#define MARIO_STATE_PICK	700
#define MARIO_STATE_DEATH	800



#define MARIO_ANI_SMALL_IDLE	0
#define MARIO_ANI_BIG_IDLE		1
#define MARIO_ANI_FIRE_IDLE		2
#define MARIO_ANI_RACCOON_IDLE	3

#define MARIO_ANI_SMALL_WALKING	4
#define MARIO_ANI_BIG_WALKING	5
#define MARIO_ANI_FIRE_WALKING	6
#define MARIO_ANI_RACCOON_WALKING	7

#define MARIO_ANI_BIG_JUMPING	8
#define MARIO_ANI_SMALL_JUMPING	9
#define MARIO_ANI_FIRE_JUMPING	10
#define MARIO_ANI_RACCOON_JUMPING	11

//#define MARIO_ANI_BIG_RUNNING	8
//#define MARIO_ANI_SMALL_RUNNING	9
//#define MARIO_ANI_FIRE_RUNNING	10
//#define MARIO_ANI_RACCOON_RUNNING	11
//

//
//#define MARIO_ANI_BIG_BRAKING	16
//#define MARIO_ANI_SMALL_BRAKING	17
//#define MARIO_ANI_FIRE_BRAKING	18
//#define MARIO_ANI_RACCOON_BRAKING	19
//
//#define MARIO_ANI_BIG_KICK_SHELL	20
//#define MARIO_ANI_SMALL_KICK_SHELL	21
//#define MARIO_ANI_FIRE_KICK_SHELL	22
//#define MARIO_ANI_RACCOON_KICK_SHELL	22
//
//
//#define MARIO_ANI_BIG_PICK_SHELL	23
//#define MARIO_ANI_SMALL_PICK_SHELL	24
//#define MARIO_ANI_FIRE_PICK_SHELL	25
//#define MARIO_ANI_RACCOON_PICK_SHELL	26
#define MARIO_ANI_DIE				12

#define	MARIO_SMALL_FORM	0
#define	MARIO_BIG_FORM		1
#define MARIO_FIRE_FORM		2
#define MARIO_RACCOON_FORM	3


#define MARIO_BIG_BBOX_WIDTH  16
#define MARIO_BIG_BBOX_HEIGHT 32

#define MARIO_SMALL_BBOX_WIDTH  16
#define MARIO_SMALL_BBOX_HEIGHT 16

#define MARIO_FIRE_BBOX_WIDTH 16
#define MARIO_FIRE_BBOX_HEIGHT 32

#define MARIO_RACCOON_BBOX_WIDTH 24
#define MARIO_RACCOON_BBOX_HEIGHT 32

#define MARIO_UNTOUCHABLE_TIME 1000
#define MARIO_LONG_JUMP_TIME 200

class Mario : public GameObject
{
	int form;
	int untouchable;
	DWORD untouchable_start;
	DWORD stack_time_start;
	int power_melter_stack;
	int jump_stack;
	DWORD long_jump_start;// pressing jumping time
	bool isInGround;
public:
	bool isJump;
	bool isPickingUp;
	Mario() : GameObject()
	{
		form = MARIO_SMALL_FORM;
		untouchable = 0;
		isJump = false;
		power_melter_stack = 1;
		form = MARIO_BIG_FORM;
	}
	Mario(float x, float y)
	{
		this->x = x;
		this->y = y;
		vx = vy = 0;
		nx = 1;
		form = MARIO_BIG_FORM;
		isEnable = true;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { form = l; }
	void SetDirect(bool nx);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void UpForm();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable);
	void FillUpPowerMelter();
	void LosePowerMelter(); 
	void Information();
	void PickUp();
	};
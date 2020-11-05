#pragma once
#include "GameObject.h"

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
#define MARIO_BBOX_SQUAT_HEIGHT	18

#define MARIO_UNTOUCHABLE_TIME 1000
//#define ams						0.4f
#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.3f
#define MARIO_SUPER_JUMP_SPEED 0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_BRAKE_DEFLECT_SPEED 0.05f
#define MARIO_GRAVITY			0.002f
#define MARIO_LOWER_GRAVITY		0.001f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define BUFF_SPEED		0.008f	// tốc độ tăng lên khi tích stack
#define POWER_METER_FULL 7		// số stack tối đa		//thời gian để tích 1 stack
#define STACK_TIME 200			//thời gian để tích 1 stack
#define MARIO_FRICTION		0.08f
#define MARIO_SUPER_JUMP_TIME 300
#define MARIO_FLYING_LIMITED_TIME 2000
#define MARIO_FLOATING_SPEED_Y	-0.02f

#define MARIO_STATE_IDLE	0
#define MARIO_STATE_WALKING	100
#define MARIO_STATE_RUNNING	200
#define MARIO_STATE_JUMPING	300
#define MARIO_STATE_SUPER_JUMPING	400
#define MARIO_STATE_BRAKING	500
#define MARIO_STATE_KICK	600
#define MARIO_STATE_PICK	700
#define MARIO_STATE_DEATH	800
#define MARIO_STATE_STOP	900
#define MARIO_STATE_SQUAT	1000
#define MARIO_STATE_SHOOT_FIREBALL	1100
#define MARIO_STATE_TAILATTACK	1200
#define MARIO_STATE_FLOATING	1300
#define MARIO_STATE_FLYING		1400


#define MARIO_ANI_SMALL_IDLE	0
#define MARIO_ANI_BIG_IDLE		1
#define MARIO_ANI_FIRE_IDLE		2
#define MARIO_ANI_RACCOON_IDLE	3

#define MARIO_ANI_SMALL_WALKING	4
#define MARIO_ANI_BIG_WALKING	5
#define MARIO_ANI_FIRE_WALKING	6
#define MARIO_ANI_RACCOON_WALKING	7

#define MARIO_ANI_SMALL_JUMPING	8
#define MARIO_ANI_BIG_JUMPING	9
#define MARIO_ANI_FIRE_JUMPING	10
#define MARIO_ANI_RACCOON_JUMPING	11

#define MARIO_ANI_SMALL_RUNNING	12
#define MARIO_ANI_BIG_RUNNING	13
#define MARIO_ANI_FIRE_RUNNING	14
#define MARIO_ANI_RACCOON_RUNNING	15

#define MARIO_ANI_SMALL_FULL_RUNNING	16
#define MARIO_ANI_BIG_FULL_RUNNING	17
#define MARIO_ANI_FIRE_FULL_RUNNING	18
#define MARIO_ANI_RACCOON_FULL_RUNNING	19


#define MARIO_ANI_SMALL_LONG_JUMPING 20
#define MARIO_ANI_BIG_LONG_JUMPING	21
#define MARIO_ANI_FIRE_LONG_JUMPING	22
#define MARIO_ANI_RACCOON_LONG_JUMPING 23

#define MARIO_ANI_SMALL_FULL_BRAKING 24
#define MARIO_ANI_BIG_FULL_BRAKING	25
#define MARIO_ANI_FIRE_FULL_BRAKING	26
#define MARIO_ANI_RACCOON_FULL_BRAKING	27

#define MARIO_ANI_SMALL_KICK_SHELL 28
#define MARIO_ANI_BIG_KICK_SHELL	29
#define MARIO_ANI_FIRE_KICK_SHELL	30
#define MARIO_ANI_RACCOON_KICK_SHELL 31

#define MARIO_ANI_SMALL_HOLD_SHELL_IDLE 32
#define MARIO_ANI_BIG_HOLD_SHELL_IDLE	33
#define MARIO_ANI_FIRE_HOLD_SHELL_IDLE	34
#define MARIO_ANI_RACCOON_HOLD_SHELL_IDLE	35

#define MARIO_ANI_SMALL_HOLD_SHELL_IDLE 36
#define MARIO_ANI_BIG_HOLD_SHELL_IDLE	37
#define MARIO_ANI_FIRE_HOLD_SHELL_IDLE	38
#define MARIO_ANI_RACCOON_HOLD_SHELL_IDLE	39

#define MARIO_ANI_BIG_DODGE	40
#define MARIO_ANI_FIRE_DODGE	41
#define MARIO_ANI_RACCOON_DODGE	42

#define MARIO_ANI_SHOOT_FIRE_BALL	43
#define MARIO_ANI_TAILATTACK		44
#define MARIO_ANI_FLOATING			45
#define MARIO_ANI_FLYING			46

#define MARIO_ANI_DIE				47




class Mario : public GameObject
{
	int form;
	int untouchable;
	DWORD untouchable_start;
	DWORD stack_time_start;
	DWORD fly_time_start;
	DWORD jump_time_start;// pressing jumping time
	int power_melter_stack;
	int jump_stack;
	bool isInGround;
	
	bool isKickShell;
	bool isDodging;
public:
		bool isPickingUp;
	bool isPressedJ;
	bool turnFriction;
	Mario() : GameObject()
	{
		form = MARIO_SMALL_FORM;
		untouchable = 0;
		//isJump = false;
		isPressedJ = false;
		power_melter_stack = 0;
		form = MARIO_BIG_FORM;
	}
	Mario(float x, float y)
	{
		this->x = x;
		this->y = y;
		vx = vy = 0;
		nx = 1;
		power_melter_stack = 0;
		form = MARIO_BIG_FORM;
		isEnable = true;
		isKickShell = false;
		isPressedJ = false;
		isInGround = true;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable);
	
	void SetState(int state);
	void SetLevel(int l) { form = l; }
	void SetDirect(bool nx);// Set hướng di chuyển cho mario

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void StartJumping() { jump_time_start = GetTickCount(); }
	void Jump();
	void SuperJump();


	//Transformation for Mario
	void UpForm();

	//Thay đổi PowerMelter
	void FillUpPowerMelter();
	void LosePowerMelter();
	
	void Information();
	void PickUp();
	void HandleCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	int GetWidth();
	int GetHeight();
	void Squat();
	int Skill();
	void Friction();
	GameObject* ShootFireBall();
	void TailAttack();
	void Float();
	void Fly();
};
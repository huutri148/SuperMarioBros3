#pragma once
#include "GameObject.h"
#include"Define.h"
#include"FireBall.h"
#include"Grid.h"
#include"RaccoonTail.h"
#include "Utils.h"

class Mario : public GameObject
{



	// Thời gian bắt đầu các trạng thái
	DWORD untouchableStart = 0;
	DWORD stackTimeStart = 0;
	DWORD flyTimeStart = 0;
	DWORD floatingTime = 0;
	DWORD shootingTime = 0;
	DWORD tailAttackTime = 0;
	DWORD transformTime = 0;
	DWORD turnRaccoonTime = 0;
	DWORD teleportTime = 0;
	DWORD kickTime = 0;

	
	int indexFireBall = 0;
	float teleportY = 0;	// Vị trí ở cổng tele
	

	//Các biến cờ ở các trạng thái
	bool isInGround ;
	bool isKickShell = false;
	bool isFlying = false;
	bool isFloating = false;
	bool isTransform = false;
	bool isTurnRaccoon = false;
	bool isReadyToJump = false;


	RaccoonTail* tail;
	
	
public:

	int untouchable = 0;
	int powerMelterStack = 0;
	int form = 0;
	int jumpStack = 0;
	bool turnToSmallForm = false;

	bool isInTeleport = false;
	bool isTeleport = false;// bật cờ khi ở cổng Tele
	bool isInExtraMap = false;
	bool isInIntroScene = false;
	bool isHitted = false;
	bool isLookUp = false;
	bool isAutoWalk = false;
	bool isTouchingPlattform = false;

	bool useSkill;
	bool pressDown = false;
	bool pressUp = false;
	bool isSquat = false;
	bool isPickingUp = false;
	bool isSwingTail = false;
	bool canBrake = false;




	bool IsFlying();
	bool IsFloating();
	bool IsInGround();
	bool IsTransform(){	return isTransform; };
	bool IsPickingShell() { return isPickingUp; };

	

	virtual void Update(DWORD dt,
		vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);

	void SetState(int state);
	void SetLevel(int l) { form = l; }
	void SetDirect(bool nx);// Set hướng di chuyển cho mario
	void SetWalkingLeft();
	void SetWalkingRight();
	void SetAutoWalk(int nx, float vx)
	{	this->nx = nx;
		this->vx = vx;
		this->isAutoWalk = true;
	};
	void SetAutoJump(float vy) { this->vy = vy; isInGround = false;}
	void SetY(float _y)
	{
		if (form == MARIO_SMALL_FORM)
			y = _y - MARIO_SMALL_BBOX_HEIGHT;
		else
			y = _y - MARIO_BIG_BBOX_HEIGHT;
	}



	// 1 số Nút Hack
	void SetTele(float x, float y) {
		this->SetLevel(MARIO_RACCOON_FORM);
		this->x = x; this->y = y;
		this->SetState(MARIO_STATE_IDLE);
	};
	void Reset();
	void UpForm();

	bool GetisPickUp() { return isPickingUp; }
	int GetWidth();
	int GetHeight();
	void Information();
	int GetPowerMelter() { return powerMelterStack; };

	//Xử Lí
	void Friction();
	void StartUntouchable() { untouchable = 1; untouchableStart = GetTickCount(); }
	void TurnOffSkill();
	void TurnBigForm();
	void TurnFireForm();
	void DecreaseForm();
	void TurnRaccoonForm();
	void HandleSwitchTime();



	//Các hành động của Mario
	void ShootFireBall(Grid* grid);
	int Skill();
	void Squat();
	void TailAttack();
	void Float();
	void Fly();
	void StartJumping();
	void Jump();
	void PickUp();
	//Thay đổi PowerMelter
	void FillUpPowerMelter();
	void LosePowerMelter();

	Mario();
};
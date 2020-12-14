#pragma once
#include "GameObject.h"
#include"Define.h"
#include"FireBall.h"
#include"Grid.h"
#include"RaccoonTail.h"
#include "Utils.h"

class Mario : public GameObject
{

	int untouchable = 0;

	// Thời gian bắt đầu các trạng thái
	DWORD untouchableStart = 0;
	DWORD stackTimeStart = 0;
	DWORD flyTimeStart = 0;
	DWORD jumpTimeStart = 0;// pressing jumping time
	DWORD floatingTime = 0;
	DWORD shootingTime = 0;
	DWORD tailAttackTime = 0;
	DWORD transformTime = 0;
	DWORD turnRaccoonTime = 0;
	DWORD teleportTime = 0;
	DWORD kickTime = 0;

	
	int indexFireBall = 0;
	int score = 0;
	int money = 0;
	int life = 4;
	int type = 0;
	
	int stageOfSwingTail;
	float teleportY = 0;// Vị trí ở cổng tele

	//Các biến cờ ở các trạng thái
	bool isInGround ;
	bool isKickShell = false;
	bool isFlying = false;
	bool isFloating = false;
	bool isTransform = false;
	bool isTurnRaccoon = false;
	bool isTeleport = false;// bật cờ khi ở cổng Tele
	RaccoonTail* tail;
	
public:
	vector<int> card;
	int form = 0;
	bool isInTeleport = false;
	bool isInExtraMap = false;
	bool isAutoWalk = false;
	bool isPressedJ;
	bool isSquat = false;
	bool isPickingUp = false;
	bool turnToSmallForm = false;
	bool isSwingTail = false;
	bool isHitted = false;
	bool isLookUp = false;
	bool canBrake = false;
	int powerMelterStack = 0;
	bool isInIntroScene = false;
	bool GetisPickUp() { return isPickingUp; }
	bool IsFlying();
	bool IsFloating();
	bool IsInGround();
	bool IsTransform()
	{
		return isTransform;
	};
	bool IsPickingShell()
	{
		return isPickingUp;
	}
	Mario();

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
	void SetAutoWalk(int nx, float vx) { this->nx = nx; this->vx = vx; this->isAutoWalk = true; };
	void SetAutoJump(float vy) { this->vy = vy; isInGround = false;}


	int GetWidth();
	int GetHeight();
	void Information();
	int GetForm() { return form; };
	int GetMoney() { return money; };
	int GetScore() { return score; };
	int GetLife() { return life; };
	int GetPowerMelter() { return powerMelterStack; };

	//Xử Lí
	void Friction();
	void StartUntouchable() { untouchable = 1; untouchableStart = GetTickCount(); }
	void UpForm();
	void ReleaseJ();
	void PressK();
	void Reset();
	void TurnBigForm();
	void TurnFireForm();
	void DecreaseForm();
	void TurnRaccoonForm();
	void HandleSwitchTime();

	//Thay đổi PowerMelter
	void FillUpPowerMelter();
	void LosePowerMelter();

	//Các hành động của Mario
	void ShootFireBall(Grid* grid);
	int Skill();
	void Squat();
	void TailAttack();
	void Float();
	void Fly();
	void StartJumping() { 	jumpTimeStart = GetTickCount(); }
	void Jump();
	void SuperJump();
	void PickUp();
	void UpdateStageOfTailAttack();
	void GainMoney(int pMoney) { money += pMoney; };
	void GainPoint(int pPoint) { score += pPoint; };
	void GainLife() { life += 1; };
	void SetTele(float x, float y) { this->SetLevel(MARIO_RACCOON_FORM); this->x = x; this->y = y; };
};
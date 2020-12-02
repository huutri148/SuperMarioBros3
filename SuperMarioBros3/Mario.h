#pragma once
#include "GameObject.h"
#include"Define.h"
#include"FireBall.h"
#include"Grid.h"



class Mario : public GameObject
{
	int form = 0;
	int untouchable = 0;

	// Thời gian bắt đầu các trạng thái
	DWORD untouchableStart = 0;
	DWORD stackTimeStart = 0;
	DWORD flyTimeStart = 0;
	DWORD jumpTimeStart = 0;// pressing jumping time
	DWORD floatingTime = 0;
	DWORD shootingTime = 0;
	DWORD tailAttackTime = 0;

	int powerMelterStack = 0;
	int indexFireBall = 0;
	int score = 0;
	int money = 0;
	int life = 4;
	int type = 0;
	int stageOfSwingTail;

	//Các biến cờ ở các trạng thái
	bool isInGround ;
	bool isKickShell = false;
	bool isFlying = false;
	bool isFloating = false;
	bool canBrake = false;
	bool isPickingUp = false;
	

public:
	// Chuyển thành hàm
	//vector<FireBall*> fireBall;
	
	bool turnFriction;
	bool isPressedJ;
	bool isSwingTail = false;
	bool GetisPickUp() { return isPickingUp; }
	bool IsFlying();
	bool IsFloating();
	bool IsInGround();
	
	Mario();

	virtual void Update(DWORD dt,
		vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT>& coEvents);


	void SetState(int state);
	void SetLevel(int l) { form = l; }
	void SetDirect(bool nx);// Set hướng di chuyển cho mario



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
	/*void HandleCollision(float min_tx, float min_ty,
		float nx, float ny, float x0, float y0);*/
	void StartUntouchable() 
	{ untouchable = 1; untouchableStart = GetTickCount(); }
	void UpForm();
	void ReleaseJ();
	void PressK();
	void Reset();
	void TurnBigForm();
	void TurnFireForm();
	void TurnRaccoonForm();

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
	void StartJumping() { jumpTimeStart = GetTickCount(); }
	void Jump();
	void SuperJump();
	void PickUp();
	bool Brake();
	void UpdateStageOfTailAttack();

	void GainMoney(int pMoney) { money += pMoney; };
	void GainPoint(int pPoint) { score += pPoint; };
	void GainLife() { life += 1; };



};
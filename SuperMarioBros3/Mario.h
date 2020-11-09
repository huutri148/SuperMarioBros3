#pragma once
#include "GameObject.h"
#include"Define.h"



class Mario : public GameObject
{
	int form;
	int untouchable;

	// Thời gian bắt đầu các trạng thái
	DWORD untouchable_start;
	DWORD stack_time_start;
	DWORD fly_time_start;
	DWORD jump_time_start;// pressing jumping time
	int power_melter_stack;

	//Các biến cờ ở các trạng thái
	bool isInGround;
	bool isKickShell;
	bool isDodging;
	bool isFlying = false;
	bool isFloating = false;
public:
	// Chuyển thành hàm
	bool isPickingUp;
	bool turnFriction;
	bool isPressedJ;
	bool IsFlying();
	bool IsFloating();
	bool IsInGround();
	
	Mario(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable);


	void SetState(int state);
	void SetLevel(int l) { form = l; }
	void SetDirect(bool nx);// Set hướng di chuyển cho mario

	int GetWidth();
	int GetHeight();
	void Information();

	//Xử Lí
	void Friction();
	void HandleCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void UpForm();
	void ReleaseJ();
	void PressK();
	void Reset();

	//Thay đổi PowerMelter
	void FillUpPowerMelter();
	void LosePowerMelter();

	//Các hành động của Mario
	GameObject* ShootFireBall();
	int Skill();
	void Squat();
	void TailAttack();
	void Float();
	void Fly();
	void StartJumping() { jump_time_start = GetTickCount(); }
	void Jump();
	void SuperJump();
	void PickUp();
};
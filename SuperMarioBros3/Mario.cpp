#include <algorithm>
#include "Utils.h"
#include "Enemy.h"
#include "Mario.h"
#include "Game.h"
#include"KoopaTroopa.h"
#include "Goomba.h"
#include"Block.h"
#include "Ground.h"
#include "Brick.h"
#include "Pipe.h"

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DEATH)
		CalcPotentialCollisions(coObjects, coEvents);
	/*if (isPickingUp == false)
	{
		shell->SetPosition(this->x, this->y);
	}*/
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);		
	
	
		x+= min_tx * dx + nx * 0.4f;		
		y += min_ty * dy + ny * 0.4f;
	

		if (nx != 0 && isPickingUp == false) vx = 0;
		if (ny != 0)
		{
			vy = 0;
			if (ny < 0)
				isInGround = true;
		}

	
		
		// Collision logic 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
				
			//if (dynamic_cast<Ground*>(e->obj)|| dynamic_cast<Brick*>(e->obj)||
			//	dynamic_cast<Pipe*>(e->obj))
			//{
			//	if(dynamic_cast<Ground*>(e->obj))
			//		DebugOut(L"Ground : %f, %f\n", e->nx,e->ny);
			//	if(dynamic_cast<Brick*>(e->obj))
			//		DebugOut(L"Brick : %f, %f\n", e->nx,e-> ny);
			//	if (nx != 0)
			//	{
			//		vx = 0;
			//	/*	x += min_tx * dx + e->nx*0.4f;
			//		y += dy;*/
			//	}
			//	if (ny != 0)
			//	{
			//		vy = 0;
			//	/*	y += min_ty * dy +e->ny* 0.4f;
			//		x += dx;*/
			//	}
			//	
			//	y += min_ty * dy + ny * 0.4f;
			//	x += min_tx * dx + nx * 0.4f;
			//	DebugOut(L"timex: %f, timey: %f\n", min_tx, min_ty);
			//}
			//if (dynamic_cast<Block*>(e->obj))
			//{
			//	DebugOut(L"Block : %f, %f\n", nx, ny);
			//	if (ny < 0 )
			//	{
			//
			//		y += min_ty * dy + ny * 0.4f;
			//		vy = 0;
			//	
			//	}
			//	else if(ny > 0)
			//	{
			//		y += dy;
			//	}
			//	x += dx;
			//}
	
									
		}	
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


	


void Mario::Render()
{
	int ani = this->form;
	if (state == MARIO_STATE_DEATH)
		ani = MARIO_ANI_DIE;
	if (vx != 0 && isInGround == true)
	{
		ani += 4;
	}
	if (!isInGround)
		ani += 8;
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(nx,x, y, alpha);
	RenderBoundingBox();
	
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING:
		vx = MARIO_WALKING_SPEED * nx;
		break;
	case MARIO_STATE_JUMPING:
		isInGround = false;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DEATH:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_RUNNING:
		vx = (MARIO_WALKING_SPEED + (BUFF_SPEED * power_melter_stack)) * nx;
		break;
	case MARIO_STATE_BRAKING:
		vx = MARIO_BRAKE_DEFLECT_SPEED * nx;
		break;
	/*case MARIO_STATE_LONG_JUMPING:
		vy = -MARIO_LONG_JUMP_SPEED_Y;
		isInGround = false;
		break;*/
	case MARIO_STATE_KICK:
		break;
	}
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable)
{
	left = x;
	top = y;

	if (form == MARIO_BIG_FORM)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (form == MARIO_SMALL_FORM)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (form == MARIO_FIRE_FORM)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
	else if (form == MARIO_RACCOON_FORM)
	{
		right = x + MARIO_RACCOON_BBOX_WIDTH;
		bottom = y + MARIO_RACCOON_BBOX_HEIGHT;
	}
}

void Mario::UpForm()
{
	int diffy = 0;
	if (form == MARIO_SMALL_FORM)
	{
		diffy = 16;

	}
	form += 1;
	if (form > MARIO_RACCOON_FORM)
	{
		diffy = -16;
		form = 0;
	}
	y -= diffy;
}
//void Mario::Jump()
//{
//	DWORD current = GetTickCount();
//	if (current - long_jump_start > MARIO_LONG_JUMP_TIME
//		&& isInGround == true		// nếu Mario chưa tiếp đất hoặc vật thì ko cho phép nhảy
//		&& long_jump_start!= 0 )	
//	{
//		this->SetState(MARIO_STATE_LONG_JUMPING);
//		isJump == false;
//		long_jump_start = 0;
//	}
//	
//	
//}
//void Mario::unJump()
//{
//	DWORD current = GetTickCount();
//	if (current - long_jump_start < MARIO_LONG_JUMP_TIME	
//		&& this->isInGround == true // nếu Mario chưa tiếp đất hoặc vật thì ko cho phép nhảy
//		&& isJump == true)		//không cho phép nhảy ngắn khi Mario đã nhảy
//	{
//		this->SetState(MARIO_STATE_JUMPING);
//		DebugOut(L"\nxyz");
//		
//	}
//	isJump = false;
//	long_jump_start = 0;
//}
void Mario::FillUpPowerMelter()// Tăng stack năng lượng của Mario
{
	DWORD current = GetTickCount();
	if (stack_time_start == 0)
	{
		stack_time_start = current;
	}
	else
	{
		if (current - stack_time_start > STACK_TIME && power_melter_stack < POWER_METER_FULL)
		{
			power_melter_stack += 1;
			stack_time_start = 0;
			DebugOut(L"Power Meter : %d", power_melter_stack);
		}	
	}
}
void Mario::LosePowerMelter()// Power Stack sẽ cạn theo thời gian
{
	if (vx == 0)
	{
		DWORD current = GetTickCount();
		if (stack_time_start == 0)
		{
			stack_time_start = current;
		}
		else
		{
			if (current - stack_time_start >  300 && power_melter_stack > 0)
			{
				power_melter_stack -= 1;
				stack_time_start = 0;
			}
		}
	}
}
void Mario::Information()
{
	DebugOut(L"\nX: %d, Y: %d", this->x, this->y);
	DebugOut(L"\nVx: %f, Vy: %f", this->vx, this->vy);
}
void Mario::PickUp()
{
	isPickingUp = true;

}
void Mario::SetDirect(bool nx)
{
	if (nx == true)
		this->nx = 1;
	else
		this->nx = -1;
}



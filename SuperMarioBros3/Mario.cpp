#include <algorithm>
#include "debug.h"
#include "Enemy.h"
#include "Mario.h"
#include "Game.h"
#include"KoopaTroopa.h"
#include "Goomba.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0)
		{
			vy = 0; isInGround = true;
		}

		// Collision logic 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Enemy*>(e->obj))
			{
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
				if (e->ny < 0)
				{
					if (enemy->IsDead() != true)
					{
						enemy->SetDie();
						isInGround = true;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (enemy->IsDead() != true)
						{
							if (form > MARIO_SMALL_FORM)
							{
								form -= 1;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
		}	
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


	


void CMario::Render()
{
	int ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
	{
		if (form == MARIO_BIG_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						ani = MARIO_ANI_BIG_RUNNING_RIGHT;
					}
					else
						ani = MARIO_ANI_BIG_WALKING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_BIG_BRAKE_RIGHT;
				}
			}
			else
			{
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
						ani = MARIO_ANI_BIG_RUNNING_LEFT;
					else
						ani = MARIO_ANI_BIG_WALKING_LEFT;
				}
				else
				{
					ani = MARIO_ANI_BIG_BRAKE_LEFT;
				}
				
			}
		}
		else if (form == MARIO_SMALL_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
						ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
					else
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SMALL_BRAKE_RIGHT;
				}
				
			}
			else
			{
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
						ani = MARIO_ANI_SMALL_RUNNING_LEFT;
					else
						ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
				else
				{
					ani = MARIO_ANI_SMALL_BRAKE_LEFT;
				}
			
			}
		}
		else if (form == MARIO_FIRE_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
						ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
					else
						ani = MARIO_ANI_FIRE_WALKING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_FIRE_BRAKE_RIGHT;
				}
				
			}
			else
			{
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
						ani = MARIO_ANI_FIRE_RUNNING_LEFT;
					else
						ani = MARIO_ANI_FIRE_WALKING_LEFT;
				}
				else
					ani = MARIO_ANI_FIRE_BRAKE_LEFT;
				
			}
		}
		else if (form == MARIO_RACCOON_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
				else ani = MARIO_ANI_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
						ani = MARIO_ANI_RACCOON_RUNNING_RIGHT;
					else
						ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_RACCOON_BRAKE_RIGHT;
				}
				
			}
			else
			{
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
						ani = MARIO_ANI_RACCOON_RUNNING_LEFT;
					else
						ani = MARIO_ANI_RACCOON_WALKING_LEFT;
				}
				else
				{
					ani = MARIO_ANI_RACCOON_BRAKE_LEFT;
				}
				
			}

		}
		int alpha = 255;
		if (untouchable) alpha = 128;
		animations[ani]->Render(x, y, alpha);

		RenderBoundingBox();
	}
	
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		/*if (jump_stack < 8)
		{*/
			vy = -MARIO_JUMP_SPEED_Y;
		/*	jump_stack++;
		}*/
	/*	isJump = true;*/
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
	
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		vx = MARIO_WALKING_SPEED + (BUFF_SPEED * power_melter_stack);
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		vx =-(MARIO_WALKING_SPEED + (BUFF_SPEED * power_melter_stack));
		nx = -1; 
		break;
	case MARIO_STATE_BRAKE_LEFT:
		vx = -(MARIO_BRAKE_DEFLECT_SPEED);
		nx = 1;
		break;
	case MARIO_STATE_BRAKE_RIGHT:
		vx = MARIO_BRAKE_DEFLECT_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_LONG_JUMP:
		vy = -MARIO_LONG_JUMP_SPEED_Y;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CMario::UpForm()
{
	float fx, fy;
	this->GetPosition(fx, fy);
	if (form < MARIO_RACCOON_FORM)
	{
		int level = form + 1;
		this->SetState(MARIO_STATE_IDLE);
		this->SetLevel(level);
		this->SetPosition(fx, 0);
		this->SetSpeed(0, 0);
	}
}
void CMario::Jump()
{
	DWORD current = GetTickCount();
	if (current - long_jump > MARIO_LONG_JUMP_TIME && isInGround == true
		&& long_jump!= 0 )
	{
		this->SetState(MARIO_STATE_LONG_JUMP);
		isJump == false;
		long_jump = 0;
	}
	
	
}
void CMario::unJump()
{
	DWORD current = GetTickCount();
	if (current - long_jump < MARIO_LONG_JUMP_TIME && this->isInGround == true && isJump == true)
	{
		this->SetState(MARIO_STATE_JUMP);
		DebugOut(L"\nxyz");
		
	}
	else
	{
		DebugOut(L"\nisInGround %d", isInGround);
		DebugOut(L"\nisJump %d", isJump);
		DebugOut(L"\nC: %d - L: %d", current, long_jump);
	}
	isJump = false;
	long_jump = 0;
}
void CMario::FillUpPowerMelter()
{
	DWORD current = GetTickCount();
	if (stack_time_start == 0)
	{
		stack_time_start = current;
	}
	else
	{
		if (current - stack_time_start > 1000 && power_melter_stack < POWER_METER_FULL)
		{
			power_melter_stack += 1;
			stack_time_start = 0;
			DebugOut(L"Power Meter : %d", power_melter_stack);
		}	
	}
}
void CMario::LosePowerMelter()
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
			if (current - stack_time_start > 1000 && power_melter_stack > 0)
			{
				power_melter_stack -= 1;
				stack_time_start = 0;
			}
		}
	}
}
void CMario::Information()
{
	DebugOut(L"\nX: %d, Y: %d", this->x, this->y);
	DebugOut(L"\nVx: %f, Vy: %f", this->vx, this->vy);
}

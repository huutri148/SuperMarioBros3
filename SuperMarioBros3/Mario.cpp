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
#include "FireBall.h"

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	//if (dt > 64)
	//	dt = 16;
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	//if (isInGround == false)
	//{
	//	if (vy > -0.2 && vy < 0.2)
	//		vy += MARIO_LOWER_GRAVITY * dt;
	//	else
	//		vy += MARIO_GRAVITY * dt;
	//}
	vy += MARIO_GRAVITY * dt;
	Friction();
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DEATH)
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
		/*isInGround = false;*/
		if (vy > 0.2)
		{	
			isInGround = false;
		}
	}
	else
	{
		float min_tx, min_ty, nex = 0, ney;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nex, ney);		
		
		float x0 = x, y0 = y;
		x = x0 + dx;
		y = y0 + dy;			
		// Collision logic 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Enemy*>(e->obj))
			{
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
				if (e->ny != 0)
				{
					if (e->ny < 0)
					{
						if (enemy->IsDead() != true)
						{
							//false: Mario giậm lên quái, true sẽ là va chạm ngang nhờ ném shell hoặc bắn
							// fireball
							enemy->SetDie(false);//false == Mario stromp in enemy
							isInGround = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
						else
						{
							HandleCollision(min_tx, min_ty, nex, ney, x0, y0);
						}
					}
				
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (enemy->IsDead() != true && enemy->IsEnable() == true)
						{
							if (form > MARIO_SMALL_FORM)
							{
								form -= 1;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DEATH);
						}
						else if (dynamic_cast<KoopaTroopa*>(enemy)->state == KOOPATROOPA_STATE_HIDING )
						{
								if (isPressedJ == true)
								{
									dynamic_cast<KoopaTroopa*>(enemy)->PickUpBy(this);
									isPickingUp = true;
								}
								else if(isPressedJ == false)
								{
									HandleCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
									dynamic_cast<KoopaTroopa*>(enemy)->isPickedUp = false;
									dynamic_cast<KoopaTroopa*>(enemy)->IsKicked(this->nx);
									this->SetState(MARIO_STATE_KICK);
								}
						}
					}	
				}
			}
			else if (!dynamic_cast<InvisibleBrick*>(e->obj))
			{
	
				HandleCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
			}
			if (dynamic_cast<Block*>(e->obj))
			{
				if (e->ny < 0)
				{
					HandleCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
					vy = 0;
				}
				else
				{
					x = x0 + dx;
					y = y0 + dy;
				}
			}
		
		}	
			

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mario::HandleCollision(float min_tx, float min_ty, float nex, float ney, float x0, float y0)
{
	if (nex != 0)
	{
		this->vx = 0;
		this->x = x0 + min_tx * this->dx + nex * 0.1f;
	}
	if (ney != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + ney * 0.1f;
		if (ney == -1)
		{
			isInGround = true;
			this->ny = 0;
		}
			
	}
}
void Mario::Render()
{
	int ani = this->form;
	if (vx == 0 && isPickingUp == true)
	{
		ani += 32;
	}
	if (state != MARIO_STATE_SQUAT)
	{
		if (vx != 0 && isInGround == true)
		{
			if ((vx > 0 && nx > 0) || (vx < 0 && nx < 0))
			{
				if (isPickingUp != true)
				{
				if (power_melter_stack < 2)
					ani += 4;
				if (power_melter_stack > 2 && power_melter_stack < 6)
					ani += 12;
				if (power_melter_stack == POWER_METER_FULL)
					ani += 16;
				}
				else
				{
					ani += 36;
				}
			}
			if ((vx > 0 && nx < 0) || ((vx < 0) && (nx > 0)))
				ani += 24;
		}
		if (!isInGround && !isPickingUp)
		{
			if (power_melter_stack > 6)
				ani += 20;
			else
				ani += 8;
			if (state == MARIO_STATE_FLOATING)
				ani = MARIO_ANI_FLOATING;
		}
	}
	else if (state == MARIO_STATE_SQUAT)
	{
		ani += 39;
	}
	else if (state == MARIO_STATE_KICK)
	{
		ani += 28;
		
	}
	if (state == MARIO_STATE_SHOOT_FIREBALL)
	{
	/*	DebugOut(L"\nC");*/
		ani = MARIO_ANI_SHOOT_FIRE_BALL;
	}
	if (state == MARIO_STATE_TAILATTACK)
	{
	/*	DebugOut(L"\nD");*/
		ani = MARIO_ANI_TAILATTACK;
	 }
	 if (state == MARIO_STATE_DEATH)
		ani = MARIO_ANI_DIE;
	/* DebugOut(L"\nAni: %d", ani);*/
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
		vx = (MARIO_WALKING_SPEED + (BUFF_SPEED * power_melter_stack)) * nx;
		break;
	case MARIO_STATE_JUMPING:
		isInGround = false;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_SUPER_JUMPING:
		isInGround = false;
		vy = -MARIO_SUPER_JUMP_SPEED;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		isKickShell = false;
		break;
	case MARIO_STATE_DEATH:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_RUNNING:
		vx = (MARIO_WALKING_SPEED + (BUFF_SPEED * power_melter_stack)) * nx;
		break;
	case MARIO_STATE_BRAKING:
		vx = MARIO_BRAKE_DEFLECT_SPEED * - nx;
		break;
	case MARIO_STATE_STOP:
		vx -= 0.01 * vx;
		break;
	case MARIO_STATE_KICK:
		isKickShell = true;
		break;
	case MARIO_STATE_FLOATING:
		this->vy -= 1.1* MARIO_GRAVITY;
		break;
	}
}
void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable)
{
	left = x;
	if (state == MARIO_STATE_SQUAT)
	{
		top = y + this->GetHeight() - MARIO_BBOX_SQUAT_HEIGHT;
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = top + MARIO_BBOX_SQUAT_HEIGHT;
	}
	else
	{
		top = y;
		if (form == MARIO_BIG_FORM)
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else if (form == MARIO_SMALL_FORM)
		{
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = top + MARIO_SMALL_BBOX_HEIGHT;
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
void Mario::FillUpPowerMelter()// Tăng stack năng lượng của Mario
{
	DWORD current = GetTickCount();
	if (stack_time_start == 0)
	{
		stack_time_start = current;
	}
	else
	{
		if (current - stack_time_start > 100 && power_melter_stack < POWER_METER_FULL)
		{
			power_melter_stack += 1;
			stack_time_start = 0;
		}	
	}
}
void Mario::LosePowerMelter()// Power Stack sẽ cạn theo thời gian
{
	if (power_melter_stack > 0)
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
	//if (vx != 0)
	//{
	//	this->SetState(MARIO_STATE_STOP);
	//	if (vx < 0.01 && vx > -0.01)
	//		vx = 0;
	//}
		
}
void Mario::Information()
{
	DebugOut(L"\nMario Vy: %f ", vy);
	DebugOut(L"\nState %d", state);
}
void Mario::PickUp()
{
	isPressedJ = true;
}
void Mario::SetDirect(bool nx)
{
	if (nx == true)
		this->nx = 1;
	else
		this->nx = -1;
}
void Mario::SuperJump()
{
	/*if (isInGround)
	{
		this->SetState(MARIO_STATE_JUMPING);
	}*/
	DWORD current = GetTickCount();
	if (current - jump_time_start > MARIO_SUPER_JUMP_TIME && isInGround == true
		&& jump_time_start != 0)
	{
		this->SetState(MARIO_STATE_SUPER_JUMPING);
		jump_time_start = 0;
	}
}
void Mario::Jump()
{
	DWORD current = GetTickCount();
	if (current - jump_time_start < MARIO_SUPER_JUMP_TIME && isInGround == true
		&& jump_time_start != 0)
	{
		this->SetState(MARIO_STATE_JUMPING);
		jump_time_start = 0;
	}
}
int  Mario::GetWidth()
{
	if (form == MARIO_RACCOON_FORM)
		return MARIO_RACCOON_BBOX_WIDTH;
	else
		return 16;
}
int Mario::GetHeight()
{
	if (form == MARIO_SMALL_FORM)
		return MARIO_SMALL_BBOX_HEIGHT;
	else
		return 32;
}
void Mario::Squat()
{
	if (form != MARIO_SMALL_FORM)
	{
			this->SetState(MARIO_STATE_SQUAT);
	}
}
int Mario::Skill()
{
	if (form == MARIO_FIRE_FORM)
	{
		return 1;
	}
	if (form == MARIO_RACCOON_FORM)
	{
		return 2;
	}
	else
		return 0;
}
void Mario::Friction()
{
	
	if (turnFriction == true)
	{
		this->vx -= this->vx * MARIO_FRICTION;
		if (abs(this->vx) < 0.007)
		{
			this->SetState(MARIO_STATE_IDLE);
			turnFriction = false;
		}
			
	}
}
GameObject* Mario::ShootFireBall()
{
	this->SetState(MARIO_STATE_SHOOT_FIREBALL);
	FireBall* fireBall;
	if(nx > 0)
		 fireBall = new FireBall(this->x + 15, this->y + 10, this->nx);
	else
		 fireBall = new FireBall(this->x - 15, this->y + 10, this->nx);
	return fireBall;
}
//void Mario::Float()
//{
//	if (this->form == MARIO_RACCOON_FORM && this->isInGround == false)
//	{
//		this->SetState(MARIO_STATE_FLOATING);
//	}
//}
void Mario::TailAttack()
{
	this->nx = (-1) * this->nx;
	this->SetState(MARIO_STATE_TAILATTACK);
}

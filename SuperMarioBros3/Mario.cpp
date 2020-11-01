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
	/*	if (vy > 0.15)
		{	
			isInGround = false;
		}*/
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
				if (e->ny < 0)
				{
					if (enemy->IsDead() != true)
					{
						enemy->SetDie(false);//false == Mario stromp in enemy
						isInGround = true;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
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

						else
						{
							if (dynamic_cast<KoopaTroopa*>(enemy)->state == KOOPATROOPA_STATE_HIDING )
							{
								if (isPressedJ == true)
								{
									dynamic_cast<KoopaTroopa*>(enemy)->PickUpBy(this);
									isPickingUp = true;
								}
								else
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
			else if(!dynamic_cast<InvisibleBrick*>(e->obj))
			{
				HandleCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
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
	//if (vx == 0 && isPickingUp == true)
	//{
	//	ani += 32;
	//}
	if (vx != 0 && isInGround == true)
	{
		if ((vx > 0 && nx > 0) || (vx < 0 && nx < 0))
		{
			/*if (isPickingUp != true)
			{*/
				if (power_melter_stack < 2)
					ani += 4;
				if (power_melter_stack > 2 && power_melter_stack < 6)
					ani += 12;
				if (power_melter_stack == POWER_METER_FULL)
					ani += 16;
				DebugOut(L"\nA");
			/*}
			else
			{
				ani += 36;
			}*/
		}
		if((vx >0 && nx < 0 )|| ((vx < 0) &&(nx > 0)))
			ani += 24;
	}
	if (!isInGround )
	{
		if (power_melter_stack > 6)
			ani += 20;
		else
			ani += 8;
		DebugOut(L"\nB");
	}
	if (state == MARIO_STATE_KICK)
	{
		ani += 28;
		DebugOut(L"\nC");
	}
	if (state == MARIO_STATE_DODGE)
	{
		ani += 39;
		DebugOut(L"\nD");
	}

	if (state == MARIO_STATE_DEATH)
		ani = MARIO_ANI_DIE;
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
		ny = -1;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	
	case MARIO_STATE_IDLE:
		vx = 0;
		isKickShell = false;
		isInGround = true;
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
	/*case MARIO_STATE_LONG_JUMPING:
		vy = -MARIO_LONG_JUMP_SPEED_Y;
		isInGround = false;
		break;*/
	case MARIO_STATE_KICK:
		isKickShell = true;
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
	DebugOut(L"\nMario State: %d ", state);
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
void Mario::Jump()
{
	if (isInGround)
	{
		this->SetState(MARIO_STATE_JUMPING);
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
void Mario::Dodge()
{
	isInGround = true;
	if (form != MARIO_SMALL_FORM && isDodging == false)
	{
		y += (this->GetHeight() - MARIO_BBOX_DODGING) - 10;
		this->SetState(MARIO_STATE_DODGE);
		isDodging = true;
	}
}
void Mario::Undodge()
{
	if (this->state == MARIO_STATE_DODGE)
	{
		y -= (this->GetHeight() - MARIO_BBOX_DODGING);
		isDodging = false;
	}
	
}

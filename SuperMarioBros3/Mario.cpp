#include <algorithm>

#include "Enemy.h"
#include "Mario.h"
#include "Game.h"
#include"KoopaTroopa.h"
#include"RaccoonLeaf.h"
#include "Goomba.h"
#include"Block.h"
#include "Ground.h"
#include "Brick.h"
#include "Pipe.h"
#include "FireBall.h"
#include "PiranhaPlant.h"
#include "Coin.h"
#include "HitEffect.h"
#include"Player.h"
#include"MovingPlattform.h"

#pragma region Các hàm cập nhật tọa độ, animation
void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable == false)
		return;

	if (GetTickCount64() - transformTime >MARIO_BIG_FORM_TRANSFORM_TIME &&
		isTransform == true)
	{
		isTransform = false;
		transformTime = 0;
		// Trường hợp biến lớn
		if (untouchable == 0)
			this->SetLevel(MARIO_BIG_FORM);
		else
			this->SetLevel(MARIO_SMALL_FORM);
	}
	if (!isAutoWalk)
	{
		if (dynamic_cast<PlayScene*>(Game::GetInstance()->GetCurrentScene()))
		{
			PlayScene* scene = (PlayScene*)Game::GetInstance()->GetCurrentScene();
			float edgeLeft = scene->GetEdgeLeft();
			float edgeRight = scene->GetEdgeRight();
			if (isInExtraMap)
			{
				edgeLeft = scene->GetEdgeLeftExtraMap();
				edgeRight = scene->GetEdgeRightExtraMap();
			}
			if (x < edgeLeft)
				x = edgeLeft;
			else if (x >= edgeRight - MARIO_BIG_BBOX_WIDTH)
				x = edgeRight - MARIO_BIG_BBOX_WIDTH;
		}
	}


	if (isTransform)
		return;
	// Calculate dx, dy 
	GameObject::Update(dt);
	// fall down slower
	if (isTeleport == false) 
	{
		if (isInGround == false)
		{
			if (vy > -MARIO_FALLING_SPEED && vy < MARIO_FALLING_SPEED)
				vy += MARIO_LOWER_GRAVITY * dt;
			else
				vy += MARIO_GRAVITY * dt;
		}
		else
			vy += MARIO_GRAVITY * dt;


		if (isFloating)
		{
			if(vy > MARIO_GRAVITY * dt)
				vy = MARIO_GRAVITY * dt;
		}
			
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die  and in teleport
	if (state != MARIO_STATE_DEATH && !isTeleport)
		CalcPotentialCollisions(coObjects, coEvents);

	HandleSwitchTime();

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (vy > MARIO_GRAVITY * dt)
			isInGround = false;
	}
	else
	{
		float min_tx, min_ty, nex = 0, ney;

		FilterCollision(coEvents, coEventsResult, 
			min_tx, min_ty,
			nex, ney);
		x += min_tx * dx + nex * 0.4f;
		y += min_ty * dy + ney * 0.4f;
		
		// Collision logic 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->ny != 0 )
			{
				if(!dynamic_cast<Item*>(e->obj) || dynamic_cast<PSwitch*>(e->obj))
					vy = 0;
				if (ney < 0)
				{
					isInGround = true;
					isFlying = false;
					flyTimeStart = 0;
					isFloating = false;
				}
			}
			if (dynamic_cast<Enemy*>(e->obj))
			{
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
				if (e->ny < 0)
				{
					if (enemy->IsDead() != true && !dynamic_cast<PiranhaPlant*>(enemy) &&
						!dynamic_cast<FirePiranhaPlant*>(enemy))
					{
						enemy->SetBeingStromped();
						Player::GetInstance()->GainPoint(100);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						vx = MARIO_WALK_DEFELCT_SPEED * nx;
					}
					else if (enemy->IsDead() == true)
					{
						if (dynamic_cast<KoopaTroopa*>(enemy))
						{
							if (enemy->state == KOOPATROOPA_STATE_HIDING)
							{
								dynamic_cast<KoopaTroopa*>(enemy)->IsKicked(nx);
								this->SetState(MARIO_STATE_KICK);
								y += dy;
							}
						}
					}
					else if (dynamic_cast<PiranhaPlant*>(enemy) ||
						dynamic_cast<FirePiranhaPlant*>(enemy))
					{
						if (untouchable == 0)
						{
							if (enemy->IsDead() != true)
							{
								if (form > MARIO_SMALL_FORM)
								{
									DecreaseForm();
								}
								else
									SetState(MARIO_STATE_DEATH);
							}
						}
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
					
						if (dynamic_cast<KoopaTroopa*>(enemy))
						{
							KoopaTroopa* koopa = dynamic_cast<KoopaTroopa*>(enemy);
							if (koopa->state ==KOOPATROOPA_STATE_HIDING)
							{
								if (useSkill == true && !koopa->isBumped ||
									useSkill == true && isInIntroScene)
								{
									koopa->PickUpBy();
									isPickingUp = true;
								}
								else if (useSkill == false
									&& enemy->vx == 0)
								{
									dynamic_cast<KoopaTroopa*>(enemy)->IsKicked(this->nx);
									this->SetState(MARIO_STATE_KICK);
									Player::GetInstance()->GainPoint(100);
								}
								else
								{
									if (form > MARIO_SMALL_FORM)
									{
										DecreaseForm();
									}
									else
										SetState(MARIO_STATE_DEATH);
								}
							}
							else
							{
								if (form > MARIO_SMALL_FORM)
								{
									DecreaseForm();
								}
								else
									SetState(MARIO_STATE_DEATH);
								enemy->ChangeDirect();
							}
						}
						else if (enemy->IsDead() != true)
						{
							if (form > MARIO_SMALL_FORM)
							{
								DecreaseForm();
							}
							else
								SetState(MARIO_STATE_DEATH);
							enemy->ChangeDirect();
						}
						else
						{
							x += dx;
						}
					}
				}
				else if (e->ny > 0)
				{
					y -= min_ty * dy + ney * 0.4f;
					if (isInIntroScene == true)
					{
						enemy->vy = -0.2f;
						enemy->vx = -0.05f;
						isHitted = true;
					}
					else
					{
						if (form > MARIO_SMALL_FORM)
						{
							DecreaseForm();
						}
						else
							SetState(MARIO_STATE_DEATH);
						enemy->ChangeDirect();
					}
				}
			}
			else if (dynamic_cast<FirePlantBullet*>(e->obj))
			{
				if (untouchable == 0)
				{
					if (form > MARIO_SMALL_FORM)
					{
						DecreaseForm();
					}
					else
						SetState(MARIO_STATE_DEATH);
					if (e->nx != 0)
						x += -(min_tx * dx + nex * 0.4f);
					if (e->ny != 0)
						y += -(min_ty * dy + ney * 0.4f);

				}
			}
			else if (dynamic_cast<Brick*>(e->obj))
			{
				Brick* brick = dynamic_cast<Brick*>(e->obj);
				if (!brick->CanUsed())
				{
					if (e->ny > 0 && nex == 0)
					{
						if (brick->Breakable() && form != MARIO_SMALL_FORM ||
							!brick->Breakable())
						{
							brick->SetEmpty();
							if (brick->state != BRICK_STATE_INACTIVE)
								Player::GetInstance()->GainPoint(100);
						}
					}
				}
				else
				{
					brick->Used();
					if (e->nx != 0)
						x += -(min_tx * dx + nex * 0.4f) ;
					if(e->ny != 0)
						y += -(min_ty * dy + ney * 0.4f) ;
					Player::GetInstance()->GainPoint(10);
					Player::GetInstance()->GainMoney(1);
				}
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
				// Fix it
				if (e->ny > 0 )
					y += -(min_ty * dy + e->ny * 0.4f) + dy - 16;
			}
			else if (dynamic_cast<Item*>(e->obj))
			{
				dynamic_cast<Item*>(e->obj)->Used();
				if (!dynamic_cast<PSwitch*>(e->obj))
				{
					if (e->ny != 0)
						y += -(min_ty * dy + ney * 0.4f) + dy;
					if (e->nx != 0)
						x += -(min_tx * dy + nex * 0.4f) + dx;
				}
			}
			else if (dynamic_cast<Pipe*>(e->obj))
			{
			//Todo: sửa lại cho thích hợp vì bị rung camera và
			// khi gặp đất rớt
				/*if (e->nx != 0 && ney != 0)
				{
					if(!isInGround)
						y += dy;
					if(e->nx < 0)
						x -= (min_tx * dx + e->nx * 0.4f) + dx;
					else if (e->nx > 0)
						x += (min_tx * dx + e->nx * 0.4f) - dx;
				}*/
				if (dynamic_cast<Pipe*>(e->obj)->type == PIPE_EXTRAMAP_PORT_TYPE_UP)
				{
					if (e->ny < 0)
					{
						// Nếu ấn phím S trên teleport
						// thì sẽ dẫn đến extraMap
						if (pressDown)
						{
							isTeleport = true;
							vy = MARIO_SPEED_TELEPORT;
							vx = 0;
							teleportY = y;
							x = e->obj->x + PIPE_BBOX_WIDTH / 4;
						}
					}
				}
				else if (dynamic_cast<Pipe*>(e->obj)->type == PIPE_EXTRAMAP_PORT_TYPE_DOWN)
				{
					if (e->ny > 0)
					{
						if (!isInGround)
						{
							isTeleport = true;
							vy = -MARIO_SPEED_TELEPORT;
							teleportY = y;
							vx = 0;
							x = e->obj->x + PIPE_BBOX_WIDTH  / 4;
						}
					}
				}
			}
			else if (dynamic_cast<Portal*>(e->obj))
			{
				x += dx;
				y += dy;
				this->SetAutoWalk(1, 0.1f);
				dynamic_cast<Portal*>(e->obj)->GetPortal();
			}
			else if (dynamic_cast<Mario*>(e->obj))
			{
				if (e->ny > 0)
				{
					this->Squat();
					isAutoWalk = false;
					vx = 0;
					y -= min_ty * dy + ney * 0.4f;
				}
				else if (e->ny < 0)
				{
					this->SetAutoJump(-0.7f);
				}
			}
			else if (dynamic_cast<MovingPlattform*>(e->obj))
			{
				if(e->ny < 0)
					e->obj->SetState(MOVING_PLATTFORM_STATE_FALLING);
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Mario::Render()
{
	float transX = 0.0f, transY = 0.0f;

	if (isEnable == false)
		return;
	int ani = -1;
	if (vx == 0 )
	{
		if (!isPickingUp)
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_IDLE;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_IDLE;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_IDLE;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_IDLE;
				break;
			}
		}
		else
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_HOLD_SHELL_IDLE;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_HOLD_SHELL_IDLE;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_HOLD_SHELL_IDLE;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_HOLD_SHELL_IDLE;
				break;
			}
		}
	}
	if (vx != 0 && isInGround == true || isAutoWalk == true)
	{
		if (isPickingUp == false)
		{
			if (powerMelterStack < POWER_MELTER_BUFF_SPEED_LEVEL)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_WALKING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_WALKING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_WALKING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_WALKING;
					break;
				}
			}
			else if(powerMelterStack >= POWER_MELTER_BUFF_SPEED_LEVEL &&
					 powerMelterStack < POWER_MELTER_FULL)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_RUNNING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_RUNNING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_RUNNING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_RUNNING;
					break;
				}
			}
			else if (powerMelterStack == POWER_MELTER_FULL)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_FULL_RUNNING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_FULL_RUNNING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_FULL_RUNNING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_FULL_RUNNING;
					break;
				}
			}
			if (canBrake)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_FULL_BRAKING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_FULL_BRAKING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_FULL_BRAKING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_FULL_BRAKING;
					break;
				}
			}
		}
		else
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_HOLD_SHELL_RUNNING;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_HOLD_SHELL_RUNNING;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_HOLD_SHELL_RUNNING;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_HOLD_SHELL_RUNNING;
				break;
			}
		}
		
	}
	if (isInGround == false )
	{
		if (!isPickingUp && isFlying == false )
		{
			if (powerMelterStack >= POWER_MELTER_BUFF_SPEED_LEVEL )
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_LONG_JUMPING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_LONG_JUMPING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_LONG_JUMPING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_LONG_JUMPING;
					break;
				}
			}
			else
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_JUMPING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_JUMPING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_JUMPING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_JUMPING;
					break;
				}
			}
		}
		else
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_HOLD_SHELL_JUMPING;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_HOLD_SHELL_JUMPING;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_HOLD_SHELL_JUMPING;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_HOLD_SHELL_JUMPING;
				break;
			}
		}
	}
	if (isSquat)
	{
		switch (form)
		{
		case MARIO_BIG_FORM:
			ani = MARIO_ANI_BIG_SQUAT;
			break;
		case MARIO_FIRE_FORM:
			ani = MARIO_ANI_FIRE_SQUAT;
			break;
		case MARIO_RACCOON_FORM:
			ani = MARIO_ANI_RACCOON_SQUAT;
			break;
		}
	}
	if (isKickShell)
	{
		switch (form)
		{
		case MARIO_SMALL_FORM:
			ani = MARIO_ANI_SMALL_KICK_SHELL;
			break;
		case MARIO_BIG_FORM:
			ani = MARIO_ANI_BIG_KICK_SHELL;
			break;
		case MARIO_FIRE_FORM:
			ani = MARIO_ANI_FIRE_KICK_SHELL;
			break;
		case MARIO_RACCOON_FORM:
			ani = MARIO_ANI_RACCOON_KICK_SHELL;
			break;
		}
	}
	if (isFloating && !isPickingUp)
	{
		ani = MARIO_ANI_FLOATING;
	}
	if (state == MARIO_STATE_DEATH)
	{
		ani = MARIO_ANI_DIE;
	}	
	if (isFlying && !isPickingUp)
	{
		ani = MARIO_ANI_FLYING;
	}
	else if (isSwingTail && !isPickingUp)
		ani = MARIO_ANI_TAILATTACK;
	else if (state == MARIO_STATE_SHOOT_FIREBALL)
		ani = MARIO_ANI_SHOOT_FIRE_BALL;
	else if (isTransform == true)
		ani = MARIO_ANI_TURN_TO_BIG_FORM;
	else if (isTurnRaccoon == true)
		ani = MARIO_ANI_TURN_TO_RACCOON;
	else if (isTeleport)
	{
		switch (form)
		{
		case MARIO_SMALL_FORM:
			ani = MARIO_ANI_SMALL_TELEPORT;
			break;
		case MARIO_BIG_FORM:
			ani = MARIO_ANI_BIG_TELEPORT;
			break;
		case MARIO_FIRE_FORM:
			ani = MARIO_ANI_FIRE_TELEPORT;
			break;
		case MARIO_RACCOON_FORM:
			ani = MARIO_ANI_RACCOON_TELEPORT;
			break;
		}
	
	
	}
	
	else if (isHitted)
		ani = MARIO_ANI_HITTED;
	else if (isLookUp)
		ani = MARIO_ANI_LOOKUP;
	int alpha = 255;
	if (form == MARIO_RACCOON_FORM && nx > 0 && !isTeleport)
		transX = RACCOONTAIL_BBOX_WIDTH;

	/// <summary>
	/// Stupid function
	/// </summary>
	if (untouchable)
	{
		if((GetTickCount64() - untouchableStart) % 2 == 0)
				alpha = 0;
		if (isTransform == true)
			alpha = 255;
		if (form == MARIO_BIG_FORM)
			ani = MARIO_ANI_TURN_TO_SMALL_FORM;
			
	}
	animation_set->at(ani)->Render(nx, round(x), round(y), alpha,transX, transY);
	//RenderBoundingBox();
}
void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING:
		vx = (MARIO_WALKING_SPEED + (BUFF_SPEED * powerMelterStack))* nx;
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
		isAutoWalk = false;
		isSquat = false;
		break;
	case MARIO_STATE_DEATH:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		break;
	case MARIO_STATE_KICK:
		isKickShell = true;
		kickTime = GetTickCount();
		break;
	case MARIO_STATE_FLOATING:
		isFloating = true;
		floatingTime = GetTickCount();
		break;
	}

}
void Mario::GetBoundingBox(float& left, float& top, float& right,
	float& bottom, bool isEnable)
{
	left = x;
	if (isSquat)
	{
		top = y + this->GetHeight() - MARIO_BBOX_SQUAT_HEIGHT;
		right = x + MARIO_BBOX_SQUAT_WIDTH;
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
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
}
#pragma endregion

#pragma region Các hành động của MARIO
void Mario::FillUpPowerMelter()// Tăng stack năng lượng của Mario
{
	DWORD current = GetTickCount();
	if ( isFlying == false && isInGround != false)
	{
		if (current - stackTimeStart > STACK_TIME 
			&& powerMelterStack < POWER_MELTER_FULL)
		{
			powerMelterStack += 1;
			stackTimeStart = current;
		}
	}
}
void Mario::LosePowerMelter()// Power Stack sẽ cạn theo thời gian
{
	if (powerMelterStack > 0 && isFlying == false)
	{
		DWORD current = GetTickCount();
		if (current - stackTimeStart > LOSE_STACK_TIME)
		{
			powerMelterStack -= 1;
			stackTimeStart = current;
		}
	}
}
void Mario::PickUp()
{
	useSkill = true;
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
	DWORD current = GetTickCount();
	if (current - jumpTimeStart > MARIO_SUPER_JUMP_TIME	&& isInGround == true &&
		jumpTimeStart != 0)
	{
		this->SetState(MARIO_STATE_SUPER_JUMPING);
		jumpTimeStart = 0;
	}
}
void Mario::Jump()
{
	DWORD current = GetTickCount();
	if (current - jumpTimeStart < MARIO_SUPER_JUMP_TIME && isInGround == true &&
		jumpTimeStart != 0)
	{
		this->SetState(MARIO_STATE_JUMPING);
		jumpTimeStart = 0;
	}
}
void Mario::Squat()
{
	if (form != MARIO_SMALL_FORM && vx == 0 || isInIntroScene )
	{
		isSquat = true;
	}

}
int Mario::Skill()
{
	if (form == MARIO_FIRE_FORM)
	{
		return MARIO_SKILL_FIREBALL;
	}
	if (form == MARIO_RACCOON_FORM)
	{
		return MARIO_SKILL_TAILATTACK;
	}
	else
		return MARIO_DO_NOTHING;
}
void Mario::Friction()
{
	this->vx -= this->vx * MARIO_FRICTION;
	if (abs(this->vx) < 0.01f && canBrake == false)
	{
		this->SetState(MARIO_STATE_IDLE);
	}
	else if (canBrake == true)
	{
		if (abs(this->vx) < 0.04f && powerMelterStack == 0)
		{
			this->SetState(MARIO_STATE_IDLE);
			canBrake = false;
		}
		else if (abs(this->vx) < 0.01f)
		{
			this->SetState(MARIO_STATE_IDLE);
			canBrake = false;
		}
	}
}
void Mario::ShootFireBall(Grid* grid)
{
	if (++indexFireBall <= MARIO_MAX_BULLET)
	{
		if (indexFireBall == 1)
		{
			shootingTime = GetTickCount();
		}
		this->SetState(MARIO_STATE_SHOOT_FIREBALL);
		FireBall* fireBall;
		if (nx > 0)
			fireBall = new FireBall(this->x + MARIO_FIRE_BBOX_WIDTH,
								this->y + MARIO_FIRE_BBOX_WIDTH / 3,
								this->nx);
		else
			fireBall = new FireBall(this->x - MARIO_FIRE_BBOX_WIDTH,
								this->y + MARIO_FIRE_BBOX_WIDTH / 3,
								this->nx);
		Unit* unit = new Unit(grid, fireBall, this->x - MARIO_FIRE_BBOX_WIDTH,
			this->y + MARIO_FIRE_BBOX_WIDTH / 3);
	}
	else
		indexFireBall = 0;
}
void Mario::Float()
{
	if (this->form == MARIO_RACCOON_FORM && this->isInGround == false && 
		isFlying == false)
	{
		isFloating = true;
		floatingTime = GetTickCount();
	}
}
void Mario::TailAttack()
{
	vx = 0;
	if (GetTickCount() - tailAttackTime > MARIO_TAIL_ATTACK_TIME)
	{
		isSwingTail = true;
		tailAttackTime = GetTickCount();

		Game* game = Game::GetInstance();
		Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
		if (nx > 0)
		{
			tail->Attack(x ,
				y + MARIO_RACCOON_BBOX_HEIGHT - RACCOONTAIL_BBOX_HEIGHT,
				this->nx, grid);
		}
		if (nx < 0)
		{
			tail->Attack(x + MARIO_TAIL_BBOX_WIDTH,
				y + MARIO_RACCOON_BBOX_HEIGHT - RACCOONTAIL_BBOX_HEIGHT,
				this->nx, grid);
		}
	}
}
void Mario::Fly()
{

	if (form == MARIO_RACCOON_FORM)
	{
		DWORD current = GetTickCount();
		if (flyTimeStart != 0 && 
			current - flyTimeStart < MARIO_FLYING_LIMITED_TIME)
		{
			this->SetState(MARIO_STATE_FLYING);
			this->vy = -MARIO_JUMP_SPEED_Y;
			this->vx = (MARIO_WALKING_SPEED +
				(BUFF_SPEED * powerMelterStack)) * nx;
			this->isFlying = true;
		}
		else if (flyTimeStart == 0 && powerMelterStack == POWER_MELTER_FULL)
		{
			flyTimeStart = current;
			this->vy = -MARIO_JUMP_SPEED_Y;
			this->vx = (MARIO_WALKING_SPEED +
				(BUFF_SPEED * powerMelterStack)) * nx;
		}
	}
}
#pragma endregion



void Mario::UpForm()
{
	int diffy = 0;
	if (form == MARIO_SMALL_FORM)
		diffy = MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;

	form += 1;
	if (form > MARIO_RACCOON_FORM)
	{
		diffy = MARIO_SMALL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
		form = 0;
	}
	y -= diffy;
}
void Mario::Information()
{
	Game* game = Game::GetInstance();
	DebugOut(L"\nMario x: %f, y: %f ", x,y);
	DebugOut(L"Get Cam X: %f", game->GetCamX());
}
int  Mario::GetWidth()
{
	if (form == MARIO_RACCOON_FORM)
		return MARIO_RACCOON_BBOX_WIDTH;
	else
		return MARIO_SMALL_BBOX_WIDTH;
}
int Mario::GetHeight()
{
	if (form == MARIO_SMALL_FORM)
		return MARIO_SMALL_BBOX_HEIGHT;
	else
		return MARIO_BIG_BBOX_HEIGHT;
}
Mario::Mario()
{
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_MARIO);
	this->SetAnimationSet(ani_set);
	vx = vy = 0;
	nx = 1;
	powerMelterStack = 0;
	form = MARIO_SMALL_FORM;
	isEnable = true;
	isKickShell = false;
	useSkill = false;
	isInGround = true;
	flyTimeStart = 0;
	isPickingUp = false;
	indexFireBall = 0;
	tail = new RaccoonTail();
	tail->isEnable = false;
}
void Mario::TurnOffSkill()
{
	isPickingUp = false;
	useSkill = false;
	if (state == MARIO_STATE_SHOOT_FIREBALL || state ==MARIO_STATE_TAILATTACK)
		SetState(MARIO_STATE_IDLE);
}
void Mario::Reset()
{
	PlayScene* playScene = (PlayScene*)Game::GetInstance()->GetCurrentScene();
	playScene->GetStartPosition(x, y);
	vx = vy = 0;
	nx = 1;
	powerMelterStack = 0;
	form = MARIO_SMALL_FORM;
	isEnable = true;
	isKickShell = false;
	useSkill = false;
	isInGround = true;
	this->SetState(MARIO_STATE_IDLE);
}
bool Mario::IsFlying()
{
	return isFlying;
}
bool Mario::IsFloating()
{
	return isFloating;
}
bool Mario::IsInGround()
{
	return isInGround;
}
void Mario::TurnBigForm()
{
	this->SetLevel(MARIO_BIG_FORM);
	transformTime = GetTickCount();
	isTransform = true;
	y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT + 2);
}
void Mario::TurnRaccoonForm()
{
	if (form != MARIO_RACCOON_FORM)
	{
		this->SetLevel(MARIO_RACCOON_FORM);
		turnRaccoonTime = GetTickCount();
		isTurnRaccoon = true;
		y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT + 2);
	}
}
void Mario::TurnFireForm()
{
	this->SetLevel(MARIO_FIRE_FORM);
	y -= (MARIO_FIRE_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT + 2);
}
void Mario::DecreaseForm()
{
	StartUntouchable();
	if (turnToSmallForm == false)
	{
		if (form == MARIO_RACCOON_FORM)
		{
			isTurnRaccoon = true;
			turnRaccoonTime = GetTickCount();
		}
		else if (form == MARIO_BIG_FORM)
		{
			isTransform = true;
			x += nx * (MARIO_BIG_BBOX_WIDTH);
			transformTime = GetTickCount();
		}
		else if (form == MARIO_FIRE_FORM)
		{
			this->SetLevel(MARIO_BIG_FORM);
		}
	}
	else
	{
		form = MARIO_BIG_FORM;
		isTransform = true;
		transformTime = GetTickCount();
	}
}
void Mario::SetWalkingRight()
{
	if (canBrake == false)
	{
		this->nx = 1;
		if (vx * nx < 0 && abs(vx) >= 0.09f)
		{
			canBrake = true;
			LosePowerMelter();
		}
		else
		{
			this->SetState(MARIO_STATE_WALKING);
			canBrake = false;
		}
	}
}
void Mario::SetWalkingLeft()
{
	if (canBrake == false)
	{
		this->nx = -1;
		if (vx * nx < 0 && abs(vx) >= 0.09f)
		{
			canBrake = true;
			LosePowerMelter();
		}
		else
		{
			this->SetState(MARIO_STATE_WALKING);
			canBrake = false;
		}
	}
}
void Mario::HandleSwitchTime()
{
	DWORD current = GetTickCount();
	if (current - tailAttackTime >
		MARIO_TAIL_ATTACK_TIME && isSwingTail == true)
	{
		isSwingTail = false;
		tail->SetState(RACCOONTAIL_STATE_INACTIVE);
	}
	
	if (current - turnRaccoonTime > MARIO_BIG_FORM_TRANSFORM_TIME &&
		isTurnRaccoon == true)
	{
		isTurnRaccoon = false;
		turnRaccoonTime = 0;
		if (untouchable == 1)
			form = MARIO_BIG_FORM;
	}

	if (current - kickTime > MARIO_KICK_LIMIT_TIME && 
		isKickShell == true)
		isKickShell = false;

	if (current - shootingTime > MARIO_SHOOTING_TIME &&
		state == MARIO_STATE_SHOOT_FIREBALL)
		this->SetState(MARIO_STATE_IDLE);

	if (current - flyTimeStart > MARIO_FLYING_LIMITED_TIME &&
		flyTimeStart != 0)
	{
		flyTimeStart = 0;
		powerMelterStack = 0;
		isFlying = false;
	}

	if (current - untouchableStart > MARIO_UNTOUCHABLE_TIME && 
		untouchable == 1)
	{
		untouchableStart = 0;
		untouchable = 0;
	}
	if (isTeleport)
	{
		if (abs(y - teleportY) > MARIO_RACCOON_BBOX_HEIGHT)
		{
			isInTeleport = true;
			teleportTime = GetTickCount();
			isTeleport = false;
		}
	}


	if (current - floatingTime > MARIO_FLOATING_TIME &&
		isFloating == true)
	{
		isFloating = false;
	}
	if (isInTeleport && current - teleportTime > MARIO_TELEPORT_TIME)
	{
		// là thời gian ở trong teleport
		isInTeleport = false;
		teleportTime = 0;
		PlayScene* playScene = (PlayScene*)Game::GetInstance()->GetCurrentScene();

		if (isInExtraMap == false)
		{
			playScene->GetExtraMapPosition(x, y);
			isInExtraMap = true;
		}
		else if (isInExtraMap == true)
		{
			playScene->GetWorldMapPosition(x, y);
			isInExtraMap = false;
		}

	}
}

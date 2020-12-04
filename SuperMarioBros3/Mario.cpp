#include <algorithm>
#include "Utils.h"
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


#pragma region Các hàm cập nhật tọa độ, animation
void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - tailAttackTime >
		MARIO_TAIL_ATTACK_TIME && isSwingTail == true)
		isSwingTail = false;
	if (GetTickCount64() - transformTime >
		MARIO_BIG_FORM_TRANSFORM_TIME && isTransform == true) 
	{
		isTransform = false;
		transformTime = 0;
		if (untouchable == 0)
		{
			this->SetLevel(MARIO_BIG_FORM);
		}
		else
		{
			this->SetLevel(MARIO_SMALL_FORM);
		}
	}
	if (GetTickCount64() - turnRaccoonTime >
		MARIO_BIG_FORM_TRANSFORM_TIME && isTurnRaccoon == true) {
		isTurnRaccoon = false;
		turnRaccoonTime = 0;
		if (untouchable == 1)
			form = MARIO_BIG_FORM;
	}
	if (isTeleport)
	{
		// Nếu vy > 0 thì MARIO đang di chuyển vào Extra map ở worldmap
		if (y - teleportY > MARIO_RACCOON_BBOX_HEIGHT && vy > 0)
		{
			isInTeleport = true;
			teleportTime = GetTickCount();
			isTeleport = false;
		}
		// nếu vy < 0 thì Mario đang di chuyển từ extramap về map cũ
		else if (teleportY - y > MARIO_RACCOON_BBOX_HEIGHT && vy < 0)
		{
			isInTeleport = true;
			teleportTime = GetTickCount();
			isTeleport = false;
		}
	}
	//DebugOut(L"\nteleportTime:\t%d", teleportTime);
	if (isInTeleport && GetTickCount() - teleportTime >
		MARIO_TELEPORT_TIME)
	{
		// có thể nói đây là thời gian ở trong teleport
		isInTeleport = false;
		teleportTime = 0;
		if (isInExtraMap == false)
		{
		///Thông tin về teleport cần được khởi tạo riêng mỗi map
		/// phải khởi tạo số này khi load map
		/// Todo: khởi tạo lại khi làm thêm map mới
			x = 2282;
			y = 520;
			isInExtraMap = true;
		}
		else if (isInExtraMap == true)
		{
			x = 2344;
			y = 362;
			isInExtraMap = false;
		}
	}
	UpdateStageOfTailAttack();
	if (dt > 64)
		dt = 16;
	if (isTransform)
		return;
	// Calculate dx, dy 
	GameObject::Update(dt);
	// fall down slower
	if (isTeleport == false) {
		if (isInGround == false)
		{
			if (vy > -0.2 &&
				vy < 0.2)
				vy += MARIO_LOWER_GRAVITY * dt;
			else
				vy += MARIO_GRAVITY * dt;
		}
		else
			vy += MARIO_GRAVITY * dt;
	}
	Friction();
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	// turn off collision when die 
	if (state != MARIO_STATE_DEATH && !isTeleport)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchableStart > MARIO_UNTOUCHABLE_TIME
		&& untouchable == 1)
	{
		untouchableStart = 0;
		untouchable = 0;
	}
	if (GetTickCount() - shootingTime> MARIO_SHOOTING_TIME
		&& state == MARIO_STATE_SHOOT_FIREBALL)
	{
		this->SetState(MARIO_STATE_IDLE);
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (vy > 0.2)
		{
			isInGround = false;
		}
	}
	else
	{
		float min_tx, min_ty, nex = 0, ney;

		FilterCollision(coEvents, coEventsResult, 
			min_tx, min_ty,
			nex, ney);
		x += min_tx * dx + nex * 0.4f;
		y += min_ty * dy + ney * 0.4f;
		if (ney != 0)
		{
			vy = 0;
			if (ney < 0)
			{
				isInGround = true;
				isFlying = false;
				isFloating = false;
			}
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
					if (enemy->IsDead() != true && !dynamic_cast<PiranhaPlant*>(enemy) &&
						!dynamic_cast<FirePiranhaPlant*>(enemy))
					{
						enemy->SetBeingStromped();
						score += 100;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						vx = nx * MARIO_WALK_DEFELCT_SPEED;
					}
					else if (enemy->IsDead() == true)
					{
						if (dynamic_cast<KoopaTroopa*>(enemy))
						{
							if (enemy->state == KOOPATROOPA_STATE_HIDING)
							{
								dynamic_cast<KoopaTroopa*>(enemy)->IsKicked(nx);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
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
									//form -= 1;
									//StartUntouchable();
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
						if (enemy->IsDead() != true)
						{
							if(!isSwingTail)
							{
								if (form > MARIO_SMALL_FORM)
								{
									//form -= 1;
									//StartUntouchable();
									DecreaseForm();
								}
								else
									SetState(MARIO_STATE_DEATH);
								enemy->ChangeDirect();
							}
							else
							{
								enemy->SetBeingSkilled(nx);
								Game* game = Game::GetInstance();
								Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
								HitEffect* effect = new HitEffect(x + this->nx * 16, y);
								Unit* unit = new Unit(grid, effect, x  + this->nx * 16, y);
							}
							
						}
						else if (dynamic_cast<KoopaTroopa*>(enemy))
						{
							KoopaTroopa* koopa = dynamic_cast<KoopaTroopa*>(enemy);
							if (koopa->state ==KOOPATROOPA_STATE_HIDING)
							{
								if (isPressedJ == true)
								{
									koopa->PickUpBy();
									isPickingUp = true;
								}
								else if (isPressedJ == false)
								{
									dynamic_cast<KoopaTroopa*>(enemy)->IsKicked(this->nx);
									this->SetState(MARIO_STATE_KICK);
									score += 100;
								}
							}
						}
						else
						{
							x += dx;
						}
					}
				}
			}
			else if (dynamic_cast<FirePlantBullet*>(e->obj))
			{
				if (untouchable == 0)
				{
					if (form > MARIO_SMALL_FORM)
					{
						//form -= 1;
						//StartUntouchable();
						DecreaseForm();
					}
					else
						SetState(MARIO_STATE_DEATH);
				}
			}
			else if (dynamic_cast<Brick*>(e->obj))
			{
				Brick* brick = dynamic_cast<Brick*>(e->obj);
				if (!brick->CanUsed())
				{
					if (e->ny > 0 && nex == 0)
					{
						//Nếu là Breakable thì Small Form ko thể làm vỡ
						// Các Brick còn lại thì có thể tác động
						if (brick->Breakable() && form != MARIO_SMALL_FORM ||
							!brick->Breakable())
						{
							brick->SetEmpty();
							if (brick->state != BRICK_STATE_INACTIVE)
								this->GainPoint(10);
						}
							
					}
					else if (e->nx != 0)
					{
						if (isSwingTail)
						{
							brick->SetEmpty();
							if (brick->state != BRICK_STATE_INACTIVE)
								this->GainPoint(10);
						}
					}
				}
				// nếu viên gạch Breakable ở trạng thái 
				// COIN hay là Empty
				else
				{
					brick->Used();
					this->GainPoint(10);
					this->GainMoney(1);
				}
			
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				x += dx;
				if (e->ny > 0 && nex == 0)
					y += dy;
			}
			else if (dynamic_cast<Item*>(e->obj))
			{
				dynamic_cast<Item*>(e->obj)->Used();
				x += dx;
				if (e->ny != 0 && nex == 0)
					y += dy;
			}
			else if (dynamic_cast<Pipe*>(e->obj))
			{
				if (dynamic_cast<Pipe*>(e->obj)->type == PIPE_EXTRAMAP_PORT_TYPE)
				{
					if (e->ny < 0)
					{
						// Nếu ấn phím S trên teleport
						// thì sẽ dẫn đến extraMap
						if (isPressS)
						{
							isTeleport = true;
							vy = MARIO_SPEED_TELEPORT;
							teleportY = y;
						}
					}
					else if (e->ny > 0)
					{
						if (!isInGround)
						{
							isTeleport = true;
							vy = - MARIO_SPEED_TELEPORT;
							teleportY = y;
						}
					}
				}
				
			}
			else if (dynamic_cast<Portal*>(e->obj))
			{
				x += dx;
				y += dy;
				card.push_back(dynamic_cast<Portal*>(e->obj)->GetPortal());
				vx = MARIO_WALK_DEFELCT_SPEED;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Mario::Render()
{
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
	if (vx != 0 && isInGround == true)
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
			else if (powerMelterStack >= POWER_MELTER_BUFF_SPEED_LEVEL
				&& powerMelterStack < POWER_METER_FULL)
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
			else if (powerMelterStack == POWER_METER_FULL)
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
	if (isInGround == false && isFlying == false)
	{
		if (!isPickingUp )
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
	if (state == MARIO_STATE_SQUAT)
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
	if (state == MARIO_STATE_KICK)
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
	if (state == MARIO_STATE_FLOATING)
	{
		ani = MARIO_ANI_FLOATING;
	}
	if (state == MARIO_STATE_DEATH)
	{
		ani = MARIO_ANI_DIE;
	}	
	if (state == MARIO_STATE_FLYING)
	{
		ani = MARIO_ANI_FLYING;
	}
	else if (isSwingTail)
		ani = MARIO_ANI_TAILATTACK;
	else if (state == MARIO_STATE_SHOOT_FIREBALL)
		ani = MARIO_ANI_SHOOT_FIRE_BALL;
	else if (isTransform == true)
		ani = MARIO_ANI_TURN_TO_BIG_FORM;
	else if (isTurnRaccoon == true)
		ani = MARIO_ANI_TURN_TO_RACCOON;
	else if (isTeleport)
		ani = MARIO_ANI_RACCOON_TELEPORT;
	int alpha = 255;
	/*DebugOut(L"Ani: %d\n", ani);*/
	if (untouchable)
	{
		if((GetTickCount64() - untouchableStart) % 2 == 0)
				alpha = 0;
		if (form == MARIO_BIG_FORM)
			ani = MARIO_ANI_TURN_TO_SMALL_FORM;
			
	}
	animation_set->at(ani)->Render(nx, round(x), round(y), alpha);
	//RenderBoundingBox();
}
void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING:
	/*	if(Brake() == false)*/
			vx = (MARIO_WALKING_SPEED +
				(BUFF_SPEED * powerMelterStack)) * nx;
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
		vx = (MARIO_WALKING_SPEED + 
			(BUFF_SPEED * powerMelterStack)) * nx;
		break;
	case MARIO_STATE_BRAKING:
		if (vx != 0)
		{
			vx = MARIO_BRAKE_DEFLECT_SPEED * -nx;
			powerMelterStack  = 0;
		}
		break;
	case MARIO_STATE_STOP:
		vx -=(float) 0.01 * vx;
		break;
	case MARIO_STATE_KICK:
		isKickShell = true;
		break;
	case MARIO_STATE_FLOATING:
		this->vy =- MARIO_FLOATING_SPEED_Y;
		isFloating = true;
		break;
	case MARIO_STATE_FLYING:
		this->vy = -MARIO_SUPER_JUMP_SPEED;
		this->vx = (MARIO_WALKING_SPEED +
			(BUFF_SPEED * powerMelterStack)) * nx;
		break;
	}

}
void Mario::GetBoundingBox(float& left, float& top, float& right,
	float& bottom, bool isEnable)
{
	left = x;
	if (state == MARIO_STATE_SQUAT)
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
			if (isSwingTail)
			{
				 if (nx < 0)
					left = x - MARIO_TAIL_BBOX_WIDTH;
				 right = left + 2 * MARIO_TAIL_BBOX_WIDTH + MARIO_RACCOON_BBOX_WIDTH;
			}
			else
			{
				if (nx > 0)
					left = x + MARIO_TAIL_BBOX_WIDTH;
				right = left + MARIO_RACCOON_BBOX_WIDTH;
			}
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
}
void Mario::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (dynamic_cast<InvisibleBrick*>(coObjects->at(i)))
		{
			continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), 
		CollisionEvent::compare);
}
#pragma endregion

#pragma region Các hành động của MARIO
void Mario::FillUpPowerMelter()// Tăng stack năng lượng của Mario
{
	this->isPressedJ = true;
	DWORD current = GetTickCount();
	if (state != MARIO_STATE_FLYING &&
		isInGround != false)
	{
		if (stackTimeStart == 0)
		{
			stackTimeStart = current;
		}
		else
		{
			if (current - stackTimeStart > STACK_TIME 
				&& powerMelterStack < POWER_METER_FULL)
			{
				powerMelterStack += 1;
				stackTimeStart = 0;
			}
		}
	}
}
void Mario::LosePowerMelter()// Power Stack sẽ cạn theo thời gian
{
	if (powerMelterStack > 0 
		&& state != MARIO_STATE_FLYING)
	{
		DWORD current = GetTickCount();
		if (stackTimeStart == 0)
		{
			stackTimeStart = current;
			
		}
		else
		{
			if (current - stackTimeStart > LOSE_STACK_TIME
			&& powerMelterStack > POWER_MELTER_MINIMUM
				&& isPressedJ != true)
			{
				powerMelterStack -= 1;
				stackTimeStart = 0;
			}
		}
	}
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
	DWORD current = GetTickCount();
	if (current - jumpTimeStart > MARIO_SUPER_JUMP_TIME
		&& isInGround == true
		&& jumpTimeStart != 0)
	{
		this->SetState(MARIO_STATE_SUPER_JUMPING);
		jumpTimeStart = 0;
	}
}
void Mario::Jump()
{
	DWORD current = GetTickCount();
	if (current - jumpTimeStart < MARIO_SUPER_JUMP_TIME && 
		isInGround == true &&
		jumpTimeStart != 0)
	{
		this->SetState(MARIO_STATE_JUMPING);
		jumpTimeStart = 0;
	/*	DebugOut(L"Is Jumping\n");*/
	}
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
		return MARIO_SKILL_FIREBALL;
	}
	if (form == MARIO_RACCOON_FORM)
	{
		return MARIO_SKILL_TAILATTCK;
	}
	else
		return MARIO_DO_NOTHING;
}
void Mario::Friction()
{

	if (turnFriction == true)
	{
		this->vx -= this->vx * MARIO_FRICTION;
		if (abs(this->vx) < 0.06)
		{
			this->SetState(MARIO_STATE_IDLE);
			turnFriction = false;
		}

	}
}
void Mario::ShootFireBall(Grid* grid)
{
	// TODO: sửa để hạn chế tạo ra nhiều đối tượng fireball
	// hạn chế việc tăng ram
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
	{
		indexFireBall = 0;
	}
	
}
void Mario::Float()
{
	if (this->form == MARIO_RACCOON_FORM && 
		this->isInGround == false && 
		vy > 0 && isFlying == false)
	{
		this->SetState(MARIO_STATE_FLOATING);
	}
}
void Mario::TailAttack()
{
	//this->nx = - this->nx;
	/*this->SetState(MARIO_STATE_TAILATTACK);*/
	if (GetTickCount() - tailAttackTime > MARIO_TAIL_ATTACK_TIME)
	{
		isSwingTail = true;
		tailAttackTime = GetTickCount();
		stageOfSwingTail = 0;
	}
}
void Mario::UpdateStageOfTailAttack()
{
	if (isSwingTail)
	{
		int stage =(int) (GetTickCount64() - tailAttackTime) /
			MARIO_EACH_STAGE_IN_SWING_TAIL_TIME;
		switch (stage)
		{
		case 0:
				if (stageOfSwingTail == 0)
				{
					x = x + nx*(MARIO_TAIL_BBOX_WIDTH - 2);
					stageOfSwingTail++;
					//DebugOut(L"[INFO] case 0, x = %f \n", x);
				}
				break;
		case 1:
				if (stageOfSwingTail == 1)
					stageOfSwingTail++;
				break;
		case 2:
				if (stageOfSwingTail == 2)
					stageOfSwingTail++;
				break;
		case 3:
				if (stageOfSwingTail == 3)
				{
					x = x - nx *( MARIO_TAIL_BBOX_WIDTH - 2) ;
					stageOfSwingTail = 0;
					//DebugOut(L"[INFO] case 3, x = %f \n", x);
				}
				break;
		}
	}
}
void Mario::Fly()
{

	if (form == MARIO_RACCOON_FORM)
	{
		DWORD current = GetTickCount();
		/*	DebugOut(L"Power melter stack: %d\n", power_melter_stack);*/
		if (powerMelterStack == POWER_METER_FULL && 
			current - flyTimeStart < MARIO_FLYING_LIMITED_TIME)
		{
			this->SetState(MARIO_STATE_FLYING);
			/*DebugOut(L"Is Flying\n");*/
			isFlying = true;
			
		}
		else if (current - flyTimeStart >
			MARIO_FLYING_LIMITED_TIME
			&& flyTimeStart != 0)
		{
			flyTimeStart = 0;
			powerMelterStack = 0;
			isFlying = false;
		}
		else if (flyTimeStart == 0 &&
			powerMelterStack == POWER_METER_FULL)
		{
			flyTimeStart = current;
			this->SetState(MARIO_STATE_FLYING);
			isInGround = false;
		}
	}
}
#pragma endregion



void Mario::UpForm()
{
	int diffy = 0;
	if (form == MARIO_SMALL_FORM)
	{
		diffy = MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;

	}
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
	//DebugOut(L"\nState %d", state);
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
	x = 16;
	y = 415;
	vx = vy = 0;
	nx = 1;
	powerMelterStack = 0;
	form = MARIO_SMALL_FORM;
	isEnable = true;
	isKickShell = false;
	isPressedJ = false;
	isInGround = true;
	flyTimeStart = 0;
	turnFriction = false;
	isPickingUp = false;
	indexFireBall = 0;
	score = 0;
	money = 0;
	life = 4;
}
void Mario::ReleaseJ()
{
	isPickingUp = false;
	isPressedJ = false;
	if (state == MARIO_STATE_SHOOT_FIREBALL ||
		state ==MARIO_STATE_TAILATTACK)
		SetState(MARIO_STATE_IDLE);
}
void Mario::PressK()
{

}
void Mario::Reset()
{
	SetPosition(16, 400);
	vx = vy = 0;
	nx = 1;
	powerMelterStack = 0;
	form = MARIO_SMALL_FORM;
	isEnable = true;
	isKickShell = false;
	isPressedJ = false;
	isInGround = true;
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
bool Mario::Brake()
{
	if (nx * vx < 0)
	{
		if (nx > 0)
		{
			vx += MARIO_WALKING_SPEED / 30;
		}
		else
		{
			vx -= MARIO_WALKING_SPEED / 30;
		}
		canBrake = true;
		powerMelterStack = 0;
		return true;
	}
	else
	{
		canBrake = false;
	}
	return false;
}
void Mario::DecreaseForm()
{
	StartUntouchable();
	if (form == MARIO_RACCOON_FORM)
	{
		isTurnRaccoon = true;
		turnRaccoonTime = GetTickCount();
	}
	if (form == MARIO_BIG_FORM)
	{
		isTransform = true; 
		transformTime = GetTickCount();
	}
}

#include "KoopaTroopa.h"
#include"Utils.h"
#include "Brick.h"
#include "Ground.h"
#include "Pipe.h"
#include "Block.h"


void KoopaTroopa::GetBoundingBox(float& left, float& top, 
	float& right, float& bottom, bool isEnable)
{
	if (isEnable == true)
	{
		left = x;
		top = y;
		right = x + KOOPATROOPA_BBOX_WIDTH;

		if (state == KOOPATROOPA_STATE_HIDING || 
			state ==KOOPATROOPA_STATE_IS_BUMPED ||
			state == KOOPATROOPA_STATE_EXIT_SHELL)
			bottom = y + KOOPATROOPA_BBOX_HEIGHT_HIDING;
		else
			bottom = y + KOOPATROOPA_BBOX_HEIGHT;

	}
	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	
}
void KoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPATROOPA_STATE_INACTIVE)
		return;
	if (GetTickCount() - time_death > KOOPATROOPA_INACTIVE_TIME &&
		this->state == KOOPATROOPA_STATE_BEING_SKILLED)
	{
		this->SetState(KOOPATROOPA_STATE_INACTIVE);
		return;
	}
	if (GetTickCount() - turnWalkingTime >
		KOOPATROOPA_TURN_WALKING_TIME &&
		this->state == KOOPATROOPA_STATE_EXIT_SHELL)
	{
		this->SetState(KOOPATROOPA_STATE_WALKING);
		this->y -= KOOPATROOPA_BBOX_HEIGHT -
			KOOPATROOPA_BBOX_HEIGHT_HIDING;
		turnWalkingTime = 0;
	}
	if (GetTickCount() - hidingTime > 
		KOOPATROOPA_EXIT_SHELL_TIME &&
		this->state == KOOPATROOPA_STATE_HIDING)
	{
		this->SetState(KOOPATROOPA_STATE_EXIT_SHELL);
		hidingTime = 0;
		turnWalkingTime = GetTickCount();
	}
	
	Enemy::Update(dt, coObjects);
	vy += KOOPATROOPA_GRAVITY * dt;

	//Xét nếu đang bị cầm ở dạng shell
	if (isPickedUp == true)
	{
		if (mario->isPickingUp)
		{
			if (mario->nx > 0)
			{
				this->x = mario->x + mario->GetWidth() - 3 ;
			}
			else
				this->x = mario->x - KOOPATROOPA_BBOX_WIDTH + 3;
			if (mario->GetHeight() > MARIO_SMALL_BBOX_HEIGHT)
				this->y = mario->y + mario->GetHeight()* 3.5 / 10;
			else
				this->y = mario->y;
			vy = 0;
		}
		else // nếu người chơi nhả nút giữ sẽ trở về Hiding
		{
			isPickedUp = false;
			this->SetState(KOOPATROOPA_STATE_HIDING);
			IsKicked(mario->nx);
		}
	
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if(state != KOOPATROOPA_STATE_BEING_SKILLED)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult,
			min_tx, min_ty, 
			nx, ny);
		float x0 = x, y0 = y;
		x = x0 + dx;
		y = y0 + dy;
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//Xử lí khi Koopa trong trạng thái bị đá và cầm
			if (isBumped == true ||isPickedUp == true)
			{
				//Bug: Khi Mario cầm shell
				if (dynamic_cast<Mario*>(e->obj))
				{
					if (isPickedUp == true)
					{
						if (e->ny != 0)
						{
							vy = 0;
							y = y0 + min_ty * dy + ny * 0.4f;
						}
						if (e->nx != 0 && isPickedUp == false)
						{
							x = x0 + min_tx * dx + nx * 0.4f;
						}
					}
				}
				if (dynamic_cast<Enemy*>(e->obj))
				{
					Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
					if (e->nx != 0 && !enemy->IsDead())
					{
						if (isPickedUp == true)
						{
							this->SetState(KOOPATROOPA_STATE_BEING_SKILLED);
						}

						enemy->SetBeingSkilled();
					}
				}
				if (isBumped == true)
				{
					if (dynamic_cast<Pipe*>(e->obj) || 
						dynamic_cast<Brick*>(e->obj) )
					{
						if (e->nx != 0)
						{
							this->vx = -this->vx;
							this->nx = -this->nx;
							x = x0 + min_tx * dx + nx * 0.4f;
						}
						if (e->ny != 0)
						{
							vy = 0;
							y = y0 + min_ty * dy + ny * 0.4f;
						}
					}
				}
				
			}
			if (dynamic_cast<Ground*>(e->obj) ||
				dynamic_cast<Brick*>(e->obj ))
			{
				
					if (e->ny != 0)
					{
						vy = 0;
						y = y0 + min_ty * dy + ny * 0.4f;
					}
					if (e->nx != 0 && isPickedUp == false)
					{
						x = x0 + min_tx * dx + nx * 0.4f;
					}
							
			}
			// sử dụng các đối tượng vô hình để chặn hướng đi của RedKoopa
			if (dynamic_cast<InvisibleBrick*>(e->obj))
			{
				//chỉ đối với RedKoopa
				if (type == 0)
				{
					if (state != KOOPATROOPA_STATE_HIDING && state != KOOPATROOPA_STATE_IS_BUMPED)
					{
						if (e->nx != 0)
						{
							this->nx = -this->nx;
							vx = -vx;
							this->x = x0 + e->t * dx + e->nx * 0.4f;
						}
						if (e->ny != 0)
						{
							vy = 0;
							this->y = y0 + min_ty * dx + e->ny * 0.4f;
						}
					}
					else if (state == KOOPATROOPA_STATE_IS_BUMPED)
					{
						if (dynamic_cast<InvisibleBrick*>(e->obj)->GetType() == INVISIBLEBRICK_TYPE_GROUND)
						{
							if (e->nx != 0)
							{
								this->nx = -this->nx;
								vx = -vx;
								this->x = x0 + e->t * dx + e->nx * 0.4f;
							}
							if (e->ny != 0)
							{
								vy = 0;
								this->y = y0 + min_ty * dx + e->ny * 0.4f;
							}
						}
					}

				}
			
			}
			if (dynamic_cast<Block*>(e->obj))
			{
				if (e->ny < 0)
				{
					this->y = y0 + min_ty * dy + e->ny * 0.4f;
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
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		//Enemy::Update(dt, coObjects);
}
void KoopaTroopa::Render()
{
	if (isEnable == true)
	{
		if (state != KOOPATROOPA_STATE_INACTIVE)
		{
			int ani = -1;

			if (state == KOOPATROOPA_STATE_WALKING)
			{
				if (type == KOOPATROOPA_TYPE_GREEN)
					ani = KOOPATROOPA_GREEN_ANI_WALKING;
				else
					ani = KOOPATROOPA_RED_ANI_WALKING;
			}
			else if (state == KOOPATROOPA_STATE_HIDING)
			{
				if (type == KOOPATROOPA_TYPE_GREEN)
					ani = KOOPATROOPA_GREEN_ANI_HIDING;
				else
					ani = KOOPATROOPA_RED_ANI_HIDING;
			}
			if (state == KOOPATROOPA_STATE_BEING_SKILLED)
			{
				if (type == KOOPATROOPA_TYPE_GREEN)
					ani = KOOPATROOPA_GREEN_ANI_DEATH;
				else
					ani = KOOPATROOPA_RED_ANI_DEATH;
			}
			if (isBumped == true)
			{
				if (type == KOOPATROOPA_TYPE_GREEN)
					ani = KOOPATROOPA_GREEN_ANI_BUMPING;
				else
					ani = KOOPATROOPA_RED_ANI_BUMPING;
			}
			if (state == KOOPATROOPA_STATE_EXIT_SHELL)
			{
				if (type == KOOPATROOPA_TYPE_GREEN)
					ani = KOOPATROOPA_GREEN_ANI_EXIT_SHELL;
				else
					ani = KOOPATROOPA_RED_ANI_EXIT_SHELL;
			}
			animation_set->at(ani)->Render(nx, x, y);
		}
		
	}

}


void KoopaTroopa::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case KOOPATROOPA_STATE_WALKING:
		vx = -KOOPATROOPA_WALKING_SPEED;
		isBumped = false;
		isPickedUp = false;
		nx = -1;
		break;
	case KOOPATROOPA_STATE_HIDING:
		isBumped = false;
		isPickedUp = false;
		vy = 0;
		vx = 0;
		break;
	case KOOPATROOPA_STATE_BEING_SKILLED:
		vy = -KOOPATROOPA_DIE_DEFLECT_SPEED;
		isBumped = false;
		isPickedUp = false;
		vx = 0;
		break;
	case KOOPATROOPA_STATE_IS_BUMPED:
		isBumped = true;
		break;
	case KOOPATROOPA_STATE_INACTIVE:
		isEnable = false;
		vx = 0;
		break;
	case KOOPATROOPA_STATE_EXIT_SHELL:
		isEnable = true;
		vx = KOOPATROPPA_EXIT_HIDING_DEFLECT_SPEED;
		vy = 0;
		break;
	}
}
bool KoopaTroopa::IsDead()
{
	if (this->state == KOOPATROOPA_STATE_HIDING || 
		this->state == KOOPATROOPA_STATE_INACTIVE)
	{
		return true;
	}
	return false;
}
void KoopaTroopa::IsKicked(int n)
{
	this->nx = n;
	this->SetState(KOOPATROOPA_STATE_IS_BUMPED);
	vx = nx *KOOPATROOPA_BUMP_SPEED;
	isBumped = true;
}
bool KoopaTroopa::IsHiding()
{
	if (this->state == KOOPATROOPA_STATE_HIDING)
		return true;
	return false;
}
void KoopaTroopa::PickUpBy(Mario* mario)
{
	isPickedUp = true;
	this->mario = mario;
}
KoopaTroopa ::KoopaTroopa(int x, int y,int _type) : Enemy(x, y)
{
	isPickedUp = false;
	//isEnable = true;
	isBumped = false;
	type = _type;
	this->SetState(KOOPATROOPA_STATE_WALKING);
}
void KoopaTroopa::SetBeingStromped()
{
	this->SetState(KOOPATROOPA_STATE_HIDING);
	hidingTime = GetTickCount();
}
void KoopaTroopa::SetBeingSkilled()
{
	this->SetState(KOOPATROOPA_STATE_BEING_SKILLED);
	time_death = GetTickCount();
}
void KoopaTroopa::EnableAgain()
{
	Enemy::EnableAgain();
	this->SetState(KOOPATROOPA_STATE_WALKING);
}

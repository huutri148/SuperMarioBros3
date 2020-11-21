#include "KoopaTroopa.h"


void KoopaTroopa::GetBoundingBox(float& left, float& top, 
	float& right, float& bottom, bool isEnable)
{
	if (isEnable == true)
	{
		left = x;
		top = y;
		right = x + KOOPATROOPA_BBOX_WIDTH;
		if (state == KOOPATROOPA_STATE_WALKING)
			bottom = y + KOOPATROOPA_BBOX_HEIGHT;
		else
			bottom = y + KOOPATROOPA_BBOX_HEIGHT_HIDING;
		
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
	HandleTimeSwitchState();
	if (state == KOOPATROOPA_STATE_INACTIVE)
		return;
	Enemy::Update(dt, coObjects);
	vy += KOOPATROOPA_GRAVITY * dt;
	
	if (state != KOOPATROOPA_STATE_WALKING)
		CanPullBack = false;
	//Xét nếu đang bị cầm ở dạng shell
	if (isPickedUp == true)
	{
		Game* game = Game::GetInstance();
		LPSCENE scence = game->GetCurrentScene();
		Mario* mario = ((PlayScene*)scence)->GetPlayer();
		if (mario->GetisPickUp())
		{
			if (mario->nx > 0)
			{
				this->x = mario->x + mario->GetWidth() - 
					KOOPATROOPA_DEFLECT_HOLDING_X;
			}
			else
				this->x = mario->x - KOOPATROOPA_BBOX_WIDTH + 
				KOOPATROOPA_DEFLECT_HOLDING_X;
			if (mario->GetHeight() > MARIO_SMALL_BBOX_HEIGHT)
				this->y = mario->y +  (float) mario->GetHeight()* 
				KOOPATROOPA_DEFLECT_HOLDING_Y;
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
		//Nếu Koopa rời khỏi mặt đất
		// kéo nó lại vị trí cuối cùng mà xảy ra đụng độ
		if (CanPullBack &&
			type == KOOPATROOPA_RED_TYPE )
		{
			if (y - lastStanding_Y >= 1.0f)
			{
				
				y -= 4;
				x -= nx * 13;
				this->ChangeDirect();
			}
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult,
			min_tx, min_ty, 
			nx, ny);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			
			LPCOLLISIONEVENT e = coEventsResult[i];
			//Xử lí khi Koopa trong trạng thái bị đá và cầm
			if (dynamic_cast<Enemy*>(e->obj))
			{
				if (isPickedUp == true || isBumped == true)
				{
					dynamic_cast<Enemy*>(e->obj)->SetBeingSkilled();
					if (isPickedUp == true)
						this->SetBeingSkilled();
				}
				else
				{
					x += dx;
				}
			}
			else if (dynamic_cast<Brick*>(e->obj))
			{
				CanPullBack = true;
				lastStanding_Y = y;
				if (e->nx != 0)
				{
					if(isBumped == true)
						dynamic_cast<Brick*>(e->obj)->SetEmpty();
					this->ChangeDirect();
				}
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				CanPullBack = true;
				lastStanding_Y = y;
				if (ny < 0)
				{
					vy = 0;
				}
				else
					x += dx;
			}
			else if (dynamic_cast<Ground*>(e->obj) || dynamic_cast<Pipe*>(e->obj))
			{
				CanPullBack = true;
				lastStanding_Y = y;
				if (nx != 0 && ny == 0)
					ChangeDirect();
				if (ny < 0)
				{
					vy = 0;
				}
				else
				{
					x += dx;
					y += dy;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
				if (type == KOOPATROOPA_GREEN_TYPE)
					ani = KOOPATROOPA_GREEN_ANI_WALKING;
				else
					ani = KOOPATROOPA_RED_ANI_WALKING;
			}
			else if (state == KOOPATROOPA_STATE_HIDING)
			{
				if (type == KOOPATROOPA_GREEN_TYPE)
					ani = KOOPATROOPA_GREEN_ANI_HIDING;
				else
					ani = KOOPATROOPA_RED_ANI_HIDING;
			}
			if (state == KOOPATROOPA_STATE_BEING_SKILLED)
			{
				if (type == KOOPATROOPA_GREEN_TYPE)
					ani = KOOPATROOPA_GREEN_ANI_DEATH;
				else
					ani = KOOPATROOPA_RED_ANI_DEATH;
			}
			if (isBumped == true)
			{
				if (type == KOOPATROOPA_GREEN_TYPE)
					ani = KOOPATROOPA_GREEN_ANI_BUMPING;
				else
					ani = KOOPATROOPA_RED_ANI_BUMPING;
			}
			if (state == KOOPATROOPA_STATE_EXIT_SHELL)
			{
				if (type == KOOPATROOPA_GREEN_TYPE)
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
		/*isPickedUp = false;*/
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
void KoopaTroopa::PickUpBy()
{
	isPickedUp = true;
}
KoopaTroopa ::KoopaTroopa(float x, float y,int _type) : Enemy(x, y)
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
	deathTime = GetTickCount();
}
void KoopaTroopa::EnableAgain()
{
	Enemy::EnableAgain();
	this->SetState(KOOPATROOPA_STATE_WALKING);
}
void KoopaTroopa::HandleTimeSwitchState()
{
	if (GetTickCount() - deathTime > KOOPATROOPA_INACTIVE_TIME &&
		this->state == KOOPATROOPA_STATE_BEING_SKILLED)
	{
		this->SetState(KOOPATROOPA_STATE_INACTIVE);
		return;
	}
	if (GetTickCount64() - turnWalkingTime >
		KOOPATROOPA_TURN_WALKING_TIME &&
		this->state == KOOPATROOPA_STATE_EXIT_SHELL)
	{			
		this->SetState(KOOPATROOPA_STATE_WALKING);
		this->y -= KOOPATROOPA_BBOX_HEIGHT -
			KOOPATROOPA_BBOX_HEIGHT_HIDING;
		if (isPickedUp == true)
		{
			this->x -= nx * KOOPATROOPA_DEFLECT_HOLDING_X;
			isPickedUp = false;
		}
			
		turnWalkingTime = 0;
	}
	if (GetTickCount64() - hidingTime >
		KOOPATROOPA_EXIT_SHELL_TIME &&
		this->state == KOOPATROOPA_STATE_HIDING)
	{
		this->SetState(KOOPATROOPA_STATE_EXIT_SHELL);
		hidingTime = 0;
		turnWalkingTime = GetTickCount();
	}

}

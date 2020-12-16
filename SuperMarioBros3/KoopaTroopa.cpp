#include "KoopaTroopa.h"


void KoopaTroopa::GetBoundingBox(float& left, float& top, 
	float& right, float& bottom, bool isEnable)
{
	left = x;
	top = y;
	right = x + KOOPATROOPA_BBOX_WIDTH;
	if (state == KOOPATROOPA_STATE_WALKING)
		bottom = y + KOOPATROOPA_BBOX_HEIGHT;
	else
		bottom = y + KOOPATROOPA_BBOX_HEIGHT_HIDING;
	
}
void KoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	HandleTimeSwitchState();
	//DebugOut(L"\nVx: %f",vx);
	if (state == KOOPATROOPA_STATE_INACTIVE || isEnable == false)
		return;
	Enemy::Update(dt, coObjects);
	vy += KOOPATROOPA_GRAVITY * dt;
	
	if (state != KOOPATROOPA_STATE_WALKING)
		CanPullBack = false;
	//Xét nếu đang bị cầm ở dạng shell
	if (isPickedUp == true && !isDead)
	{
		Game* game = Game::GetInstance();
		LPSCENE scence = game->GetCurrentScene();
		Mario* mario = NULL;
		if(dynamic_cast<PlayScene*>(scence))
			mario = ((PlayScene*)scence)->GetPlayer();
		if (dynamic_cast<IntroScene*>(scence))
			mario = ((IntroScene*)scence)->GetHoldingPlayer();
		if (mario != NULL)
		{
			if (mario->GetisPickUp())
			{
				if (mario->nx > 0)
				{
					this->x = mario->x + mario->GetWidth() -
						KOOPATROOPA_DEFLECT_HOLDING_X;
					if(mario->form == MARIO_RACCOON_FORM)
					this->x = mario->x + RACCOONTAIL_BBOX_WIDTH+ mario->GetWidth() -
							KOOPATROOPA_DEFLECT_HOLDING_X;
				}
				else
					this->x = mario->x - KOOPATROOPA_BBOX_WIDTH +
					KOOPATROOPA_DEFLECT_HOLDING_X;
				if (mario->GetHeight() > MARIO_SMALL_BBOX_HEIGHT)
					this->y = mario->y + (float)mario->GetHeight() *
					KOOPATROOPA_DEFLECT_HOLDING_Y;
				else
					this->y = mario->y;
				vx = 0;
				vy = 0;
			}
			if(mario->isPressedJ == false) // nếu người chơi nhả nút giữ sẽ trở về Hiding
			{
				isPickedUp = false;
				mario->isPickingUp = false;
				this->SetState(KOOPATROOPA_STATE_HIDING);
				IsKicked(mario->nx);
			}
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if(isDead != true)
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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (ny != 0 && !dynamic_cast<Mario*>(e->obj))
			{
				vy = 0;
				if (hidingTime != 0 && !isBumped || forceShell == true)
					vx = 0;
			}
			//Xử lí khi Koopa trong trạng thái bị đá và cầm
			if (dynamic_cast<Enemy*>(e->obj))
			{
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
				if (enemy->isDead == false)
				{
					if (isPickedUp == true || isBumped == true)
					{
						enemy->SetBeingSkilled(this->nx);
						enemy->SetDead();

						if (isPickedUp == true)
						{
							this->SetState(KOOPATROOPA_STATE_DEATH);
						}
					}
					else
					{
						if (e->ny != 0)
						{
							y -= min_ty * dy + ny * 0.4f;
						}
						x += dx;
					}
				}
				else
				{
					if (e->ny != 0)
					{
						y -= min_ty * dy + ny * 0.4f;
					}					
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
				if(e->ny < 0 )
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
			else if (dynamic_cast<Item*>(e->obj))
			{
				x += dx;
				y += dy;
			}
			else if (dynamic_cast<Mario*>(e->obj))
			{
				if (e->ny > 0)
				{
					vy = 0;
					y -= min_ty * dy + ny * 0.4f;
				}
				if (e->nx != 0)
				{
					x -= min_tx * dx + nx * 0.4f;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void KoopaTroopa::Render()
{
	if (state != KOOPATROOPA_STATE_INACTIVE || isEnable == false)
	{
		int ani = -1;

		if (state == KOOPATROOPA_STATE_WALKING)
		{
			if (type == KOOPATROOPA_GREEN_TYPE)
				ani = KOOPATROOPA_GREEN_ANI_WALKING;
			else
				ani = KOOPATROOPA_RED_ANI_WALKING;
		}
		else if (state == KOOPATROOPA_STATE_HIDING || state == KOOPATROOPA_STATE_DEATH)
		{
			if (type == KOOPATROOPA_GREEN_TYPE)
				ani = KOOPATROOPA_GREEN_ANI_HIDING;
			else
				ani = KOOPATROOPA_RED_ANI_HIDING;
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
		animation_set->at(ani)->Render(nx,ny, round(x), round(y));
	}
}
void KoopaTroopa::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case KOOPATROOPA_STATE_WALKING:
		vx = -KOOPATROOPA_WALKING_SPEED;
		ny = -1;
		isBumped = false;
		nx = -1;
		break;
	case KOOPATROOPA_STATE_HIDING:
		isBumped = false;
		isPickedUp = false;
		vy = 0;
		vx = 0;
		break;
	case KOOPATROOPA_STATE_DEATH:
		this->nx = -nx;
		deathTime = GetTickCount();
		vx = nx * KOOPATROOPA_DIE_DEFLECT_SPEED_X;
		vy = -KOOPATROOPA_DIE_DEFLECT_SPEED_Y;
		isDead = true;
		break;
	case KOOPATROOPA_STATE_INACTIVE:
		x = entryX;
		y = entryY;
		CanPullBack = false;
		hidingTime = 0;
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
	if (this->state == KOOPATROOPA_STATE_HIDING && !isBumped || 
		this->state == KOOPATROOPA_STATE_INACTIVE)
	{
		return true;
	}
	return false;
}
void KoopaTroopa::IsKicked(int n)
{
	this->nx = n;
	isBumped = true;
	forceShell = false;
	vx = nx *KOOPATROOPA_BUMP_SPEED;
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
	isEnable = true;
	isBumped = false;
	type = _type;
	this->SetState(KOOPATROOPA_STATE_WALKING);
}
KoopaTroopa::KoopaTroopa()
{
	type = 1;
	this->SetState(KOOPATROOPA_STATE_WALKING);
	this->isEnable = true;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(4);
	this->SetAnimationSet(ani_set);
}
void KoopaTroopa::SetBeingStromped()
{
	if (state != KOOPATROOPA_STATE_HIDING)
	{
		Game* game = Game::GetInstance();
		LPSCENE scene = game->GetCurrentScene();
		if (dynamic_cast<PlayScene*>(scene))
		{
			Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
			PointEffect* effect = new PointEffect(x, y, POINT_TYPE_100);
			Unit* unit = new Unit(grid, effect, x, y);
		}
	}
	this->SetState(KOOPATROOPA_STATE_HIDING);
	hidingTime = GetTickCount();
}
void KoopaTroopa::SetBeingSkilled(int _nx)
{
	this->nx = _nx;
	if (state != KOOPATROOPA_STATE_HIDING)
	{
		Game* game = Game::GetInstance();
		LPSCENE scene = game->GetCurrentScene();
		if (dynamic_cast<PlayScene*>(scene))
		{
			Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
			PointEffect* effect = new PointEffect(x, y, POINT_TYPE_100);
			Unit* unit = new Unit(grid, effect, x, y);
		}
	}
	this->SetState(KOOPATROOPA_STATE_HIDING);
	hidingTime = GetTickCount();
	vx = nx *KOOPATROOPA_DIE_DEFLECT_SPEED_X;
	vy = -KOOPATROOPA_DIE_DEFLECT_SPEED_Y;
	ny = 1;
	isBumped = false;
	isPickedUp = false;			
}
void KoopaTroopa::HandleTimeSwitchState()
{
	if (GetTickCount() - deathTime > KOOPATROOPA_DEATH_TIME &&
		isDead == true)
	{
		isEnable = true;
		return;
	}
	if (forceShell == false)
	{
		// Ở trạng thái hiding chuyển sang walking
		if (GetTickCount64() - turnWalkingTime >
			KOOPATROOPA_TURN_WALKING_TIME &&
			this->state == KOOPATROOPA_STATE_EXIT_SHELL)
		{
			this->SetState(KOOPATROOPA_STATE_WALKING);
			this->y -= KOOPATROOPA_BBOX_HEIGHT -
				KOOPATROOPA_BBOX_HEIGHT_HIDING;

			// Nếu mario còn giữ sẽ đẩy BoundingBox ra một chút
			if (isPickedUp == true)
			{
				this->x -= nx * KOOPATROOPA_DEFLECT_HOLDING_X;
				isPickedUp = false;
			}
			turnWalkingTime = 0;
		}

		// Chuyển sang Exit shell 
		if (GetTickCount64() - hidingTime >
			KOOPATROOPA_EXIT_SHELL_TIME && !isBumped &&
			this->state == KOOPATROOPA_STATE_HIDING)
		{
			this->SetState(KOOPATROOPA_STATE_EXIT_SHELL);
			hidingTime = 0;
			turnWalkingTime = GetTickCount();
		}
		// Khi vận tốc về 0 sẽ tự động tính thời gian hiding
		if (this->state == KOOPATROOPA_STATE_HIDING
			&& vx == 0 && hidingTime == 0 && forceShell == false)
		{
			hidingTime = GetTickCount();
			isBumped = false;
		}
	}
}

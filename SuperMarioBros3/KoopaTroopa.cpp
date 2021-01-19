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
	HandleTimeSwitchState(coObjects);


	if (state == KOOPATROOPA_STATE_INACTIVE || isEnable == false)
		return;

	Enemy::Update(dt, coObjects);

	if (!isBumped)
	{
		vy += KOOPATROOPA_GRAVITY * dt;
		if (vy > KOOPATROOPA_MAX_GRAVITY * 3)
			vy = KOOPATROOPA_MAX_GRAVITY * 3;
	}
	else
	{
		vy += KOOPATROOPA_BUMPED_GRAVITY * dt;
		if (vy > KOOPATROOPA_MAX_GRAVITY )
			vy = KOOPATROOPA_MAX_GRAVITY;
	}
	
		
	
	if (state != KOOPATROOPA_STATE_WALKING)
		canPullBack = false;
	
	
	
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
				}
				else
					this->x = mario->x - KOOPATROOPA_BBOX_WIDTH +
					KOOPATROOPA_DEFLECT_HOLDING_X;

				if (mario->GetHeight() > MARIO_SMALL_BBOX_HEIGHT)
					this->y = mario->y + (float)mario->GetHeight() *
					KOOPATROOPA_DEFLECT_HOLDING_Y;
				else
					this->y = mario->y;

				if (mario->isTeleport)
				{
					x = mario->x;
				}

				vx = 0;
				vy = 0;
			}
			if(mario->useSkill == false && !mario->isTeleport) // nếu người chơi nhả nút giữ sẽ trở về Hiding
			{
				isPickedUp = false;
				mario->isPickingUp = false;

				// Sử dụng ở IntroScene
				if(!mario->isInIntroScene)
					mario->SetState(MARIO_STATE_KICK);
				this->SetState(KOOPATROOPA_STATE_HIDING);


				IsKicked(mario->nx);
				CheckWhenHidhing(coObjects);
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
		if (canPullBack &&
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
			if (ny != 0 && (!dynamic_cast<Mario*>(e->obj) || dynamic_cast<Brick*>(e->obj)))
			{
				if (dynamic_cast<Brick*>(e->obj))
				{
					if (!dynamic_cast<Brick*>(e->obj)->CanUsed())
						vy = 0;
				}
				else 
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
						enemy->SetDead();
						enemy->SetBeingSkilled(this->nx);
						if (isPickedUp == true)
						{
							this->SetState(KOOPATROOPA_STATE_DEATH);
						}
						if (e->ny != 0)
						{
							y -= (min_ty * dy + e->ny * 0.4f);
						}
					}
					else
					{
						if (e->ny != 0)
						{
							y -= (min_ty * dy + e->ny * 0.4f);
						}
						if(e->nx != 0)
							x += -(min_tx * dx + nx * 0.4f) + dx;
					}
				}
				else
				{
					if (e->ny > 0)
					{
						y += -(min_ty * dy + ny * 0.4f) + dy;
					}
					x += dx;
				}
			}
			else if (dynamic_cast<Brick*>(e->obj))
			{
				
				if (!dynamic_cast<Brick*>(e->obj)->CanUsed())
				{
					canPullBack = true;
					lastStanding_Y = y;
					if (e->nx != 0 && ny == 0)
					{
						if (isBumped == true)
							dynamic_cast<Brick*>(e->obj)->SetEmpty(true);
						this->ChangeDirect();
					}
					if (e->ny < 0)
					{
						if (e->obj->vy != 0)
							this->SetBeingSkilled(-this->nx);
					}
				}
				else
				{
					if (e->ny != 0)
						y += -(min_ty * dy + e->ny * 0.4f) + dy;
					if (e->nx != 0)
						x += -(min_tx * dy + e->nx * 0.4f) + dx;
					nx = 1;
				}
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				canPullBack = true;
				lastStanding_Y = y;
				if(e->ny < 0 )
				{
					vy = 0;
				}
				else
					x += dx;
			}
			else if (dynamic_cast<Ground*>(e->obj) || dynamic_cast<Pipe*>(e->obj) ||
					dynamic_cast<MovingPlattform*>(e->obj))
			{
				canPullBack = true;
				lastStanding_Y = y;
				if (nx != 0 && ny == 0)
				{
					if (dynamic_cast<MovingPlattform*>(e->obj))
					{
						if(isBumped)
							ChangeDirect();
						else 
							x += dx;
					}
					else
						ChangeDirect();
				}
					
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
					if (dynamic_cast<Mario*>(e->obj)->untouchable == 0)
						x -= min_tx * dx + nx * 0.4f;
					else
						x += -(min_tx * dx + nx * 0.4f) + dx;
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
	//RenderBoundingBox();
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
		canPullBack = false;
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
	if (isInItroScene == true)
	{
		vx = nx * KOOPATROOPA_BUMP_SPEED;
		forceShell = false;
	}
	else
		vx = nx * KOOPATROOPA_FAST_BUMP_SPEED;
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
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_KOOPA);
	this->SetAnimationSet(ani_set);
	isPickedUp = false;
	isEnable = true;
	isBumped = false;
	type = _type;
	this->SetState(KOOPATROOPA_STATE_WALKING);
}
KoopaTroopa::KoopaTroopa(int type)
{
	this->type = type;
	this->SetState(KOOPATROOPA_STATE_WALKING);
	this->isEnable = true;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_KOOPA);
	this->SetAnimationSet(ani_set);
}


void KoopaTroopa::SetBeingStomped()
{
	if (state != KOOPATROOPA_STATE_HIDING)
	{
		this->GainScore(100);
	}
	this->SetState(KOOPATROOPA_STATE_HIDING);
	hidingTime = GetTickCount();
}


void KoopaTroopa::SetBeingSkilled(int _nx)
{
	this->nx = _nx;
	if (state != KOOPATROOPA_STATE_HIDING)
	{
		this->GainScore(100);
	}
	this->SetState(KOOPATROOPA_STATE_HIDING);
	hidingTime = GetTickCount();
	vx = nx *KOOPATROOPA_DIE_DEFLECT_SPEED_X;
	vy = -KOOPATROOPA_DIE_DEFLECT_SPEED_Y;
	ny = 1;
	isBumped = false;
	isPickedUp = false;
	if (isDead == true)
		deathTime = GetTickCount();
}



void KoopaTroopa::HandleTimeSwitchState(vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount() - deathTime > KOOPATROOPA_DEATH_TIME &&
		isDead == true)
	{
		isEnable = false;
		return;
	}
	if (forceShell == false)// forceShell dùng để chặn cho trở lại dạng Walking (dùng ở introScene)
	{
		// Ở trạng thái hiding chuyển sang walking
		if (GetTickCount64() - turnWalkingTime > KOOPATROOPA_TURN_WALKING_TIME &&
			this->state == KOOPATROOPA_STATE_EXIT_SHELL)
		{
			// Nếu mario còn giữ sẽ đẩy BoundingBox ra một chút
			if (isPickedUp == true)
			{
				this->x -= nx * KOOPATROOPA_DEFLECT_HOLDING_X;
				isPickedUp = false;
				Game* game = Game::GetInstance();
				LPSCENE scence = game->GetCurrentScene();
				Mario* mario = NULL;
				if (dynamic_cast<PlayScene*>(scence))
				{
					mario = ((PlayScene*)scence)->GetPlayer();
					mario->isPickingUp = false;
				}
			}
			turnWalkingTime = 0;
			// Trường hợp rùa bị cầm và tỉnh lại trong Pipe, Gạch hoặc đất
			if (!CheckWhenHidhing(coObjects))
			{
				this->SetState(KOOPATROOPA_STATE_WALKING);
				this->y -= KOOPATROOPA_BBOX_HEIGHT -
					KOOPATROOPA_BBOX_HEIGHT_HIDING;
			}
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
bool KoopaTroopa::CheckAABB(GameObject* obj)
{
	float l, t, r, b;
	float l1, t1, r1, b1;

	this->GetBoundingBox(l, t, r, b, isEnable);
	obj->GetBoundingBox(l1, t1, r1, b1, obj->isEnable);


	// Sửa lại hàm sao cho chỉ cần 3 cạnh chèn ở trong coObject sẽ trả true
	bool d1 = (r1 - l < GetWidth() && r1 - l > 0 || (r - l1 < GetWidth() && r - l1 > 0));
	bool d2 = (b - t1 < KOOPATROOPA_BBOX_HEIGHT_HIDING && b - t1 > 0) ||
		(t - b1 < KOOPATROOPA_BBOX_HEIGHT_HIDING && t - b1 > 0);
	return d1 && d2;
}




bool KoopaTroopa::CheckWhenHidhing(vector<LPGAMEOBJECT>* coObjects)
{
	if (state != KOOPATROOPA_STATE_WALKING && !isDead) // Chỉ kiểm tra khi ở dạng Shell
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<Pipe*>(coObjects->at(i)) || dynamic_cast<Ground*>(coObjects->at(i)) ||
				dynamic_cast<Brick*>(coObjects->at(i)))
			{
				if (this->CheckAABB(coObjects->at(i)))
				{
					this->SetState(KOOPATROOPA_STATE_DEATH);
					ny = 1;
					vx = 0;
					Game* game = Game::GetInstance();
					LPSCENE scene = game->GetCurrentScene();
					this->GainScore(100);
					return true;
				}
			}
		}
		return false;
	}
	return false;
}
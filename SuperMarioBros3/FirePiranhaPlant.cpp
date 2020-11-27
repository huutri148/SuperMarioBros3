#include "FirePiranhaPlant.h"
#include "Ground.h"
#include "Brick.h"
void FirePiranhaPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	if (state != FIREPIRANHAPLANT_STATE_DEATH)
	{
		left = x;
		top = y;
		right = x + PIRANHAPLANT_BBOX_WIDTH;
		bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = x + FIREPIRANHAPLANT_BBOX_DEATH_WIDTH;
		bottom = y + FIREPIRANHAPLANT_BBOX_DEATH_HEIGHT;
	}
}
void FirePiranhaPlant::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	GetDirect();
	HandleTimeSwitchState();
	if (state == FIREPIRANHAPLANT_STATE_INACTIVE)
		return;
	Enemy::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (!this->IsDead())
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
		y = y0 + dy;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					isOutOfPipe = false;
					switchTime = GetTickCount();
					vy = 0;
					this->y = y0 + e->t * dy + e->ny * 0.4f;
				}

			}
			else if (dynamic_cast<InvisibleBrick*>(e->obj))
			{
				if (e->ny != 0)
				{
					isOutOfPipe = true;
					this->SetState(FIREPIRANHAPLANT_STATE_SHOOTING);
					switchTime = GetTickCount();
					this->y = y0 + e->t * dy + e->ny * 0.4f;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void FirePiranhaPlant::Render()
{
	if (state != FIREPIRANHAPLANT_STATE_INACTIVE)
	{
		int ani = FIREPIRANHAPLANT_GREEN_ANI_DARTING_TOP;
		if (state == FIREPIRANHAPLANT_STATE_DARTING)
		{
			if (shootInGround == false)
			{
				if (type != FIREPIRANHAPLANT_RED_TYPE)
					ani = FIREPIRANHAPLANT_GREEN_ANI_DARTING_TOP;
				else
					ani =FIREPIRANHAPLANT_RED_ANI_DARTING_TOP;
			}
			else
			{
				if (type != FIREPIRANHAPLANT_RED_TYPE)
					ani = FIREPIRANHAPLANT_GREEN_ANI_DARTING_BOTTOM;
						else
							ani = FIREPIRANHAPLANT_RED_ANI_DARTING_BOTTOM;
					}
						
				}
				else if (state == FIREPIRANHAPLANT_STATE_SHOOTING)
				{
					if (shootInGround == false)
					{
						if (type != FIREPIRANHAPLANT_RED_TYPE)
							ani = FIREPIRANHAPLANT_GREEN_ANI_SHOOTING_TOP;
						else
							ani = FIREPIRANHAPLANT_RED_ANI_SHOOTING_TOP;
					}
					else
					{
						if (type != FIREPIRANHAPLANT_RED_TYPE)
							ani = FIREPIRANHAPLANT_GREEN_ANI_SHOOTING_BOTTOM;
						else
							ani = FIREPIRANHAPLANT_RED_ANI_SHOOTING_BOTTOM;
					}
				}
				else if (state == FIREPIRANHAPLANT_STATE_DEATH)
					ani = FIREPIRANHAPLANT_ANI_DEATH;
				animation_set->at(ani)->Render(nx, round(x),round( y));
			}
}
FirePiranhaPlant::FirePiranhaPlant(float x, float y, int _type) :Enemy(x, y)
{
	this->state = FIREPIRANHAPLANT_STATE_DARTING;
	vy = FIREPIRANHAPLANT_DARTING_SPEED;
	this->isOutOfPipe = true;
	this->type = _type;
}
void FirePiranhaPlant::SetState(int _state)
{
	Enemy::SetState(_state);
	switch (_state)
	{
	case FIREPIRANHAPLANT_STATE_DARTING:
		isShooted = false;
		if (isOutOfPipe == false)
		{
			vy = -PIRANHAPLANT_DARTING_SPEED;
		}
		else
			vy = PIRANHAPLANT_DARTING_SPEED;
		break;
	case FIREPIRANHAPLANT_STATE_SHOOTING:
		vy = 0;
		break;
	case FIREPIRANHAPLANT_STATE_DEATH:
		vy = 0;
		break;
	case FIREPIRANHAPLANT_STATE_INACTIVE:
	/*	isEnable = false;*/
		x = entryX;
		y = entryY;
		break;
	}
}
bool FirePiranhaPlant::IsDead()
{
	if (state == FIREPIRANHAPLANT_STATE_DEATH ||
		state == FIREPIRANHAPLANT_STATE_INACTIVE)
	{
		return true;
	}
	return false;
}
void FirePiranhaPlant::SetBeingSkilled(int nx)
{
	this->SetState(FIREPIRANHAPLANT_STATE_DEATH);
	y += FIREPIRANHAPLANT_BBOX_HEIGHT -
		FIREPIRANHAPLANT_BBOX_DEATH_HEIGHT;
	deathTime = GetTickCount();
}
void FirePiranhaPlant::SetBeingStromped()
{

}

void FirePiranhaPlant::HandleTimeSwitchState()
{
	DWORD current = GetTickCount();
	// Chuyển trạng thái từ death sang InAcTive
	// sử dụng ani 
	// nên chuyển thành effect riêng
	if (state == FIREPIRANHAPLANT_STATE_DEATH &&
		current - deathTime >
		FIREPIRANHAPLANT_INACTIVE_TIME
		&& deathTime != 0)
	{
		isEnable = false;
		deathTime = 0;
	}
	// Chuyển trạng thái từ lao ngoài Pipe khi va chạm với Ground
	if (state == FIREPIRANHAPLANT_STATE_DARTING &&
		current - switchTime >
		FIREPIRANHAPLANT_SWITCH_TIME && switchTime != 0)
	{
		this->SetState(FIREPIRANHAPLANT_STATE_DARTING);
	}
	// Bắn trong một khoảng thời gian nhất định
	if (state == FIREPIRANHAPLANT_STATE_SHOOTING &&
	current - switchTime >	FIREPIRANHAPLANT_SHOOTING_TIME 
		&& current - switchTime < FIREPIRANHAPLANT_SWITCH_TIME
		&& isShooted== false)
	{
		/*Shooting();*/
		canShoot = true;
	}
	// Chuyển trạng thái khi ở trên Pipe sang trạng thái đợi bắn
	if (state == FIREPIRANHAPLANT_STATE_SHOOTING &&
		current - switchTime >
		FIREPIRANHAPLANT_SWITCH_TIME && switchTime != 0)
	{
		this->SetState(FIREPIRANHAPLANT_STATE_DARTING);
	}
	
}
void FirePiranhaPlant::Shooting(Grid* grid)
{
	Game* game = Game::GetInstance();
	LPSCENE scence = game->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	float mX, mY;
	mario->GetPosition(mX, mY);
	float diffX = mX - x, diffY = mY - y;
	if ((diffX > FIREPIRANHAPLANT_FREEFIRE_MIN_ZONE_X
		&& diffX < FIREPIRANHAPLANT_FREEFIRE_MAX_ZONE_X) ||
		(diffX < - FIREPIRANHAPLANT_FREEFIRE_MIN_ZONE_X
			&& diffX > -FIREPIRANHAPLANT_FREEFIRE_MAX_ZONE_X))
	{
		int direct = 1;
		if (abs(diffX) > FIREPIRANHAPLANT_TARGET_X)
		{
			if (diffY > 0)
				direct = FIREBULLET_DIRECT_1;
			else
				direct = FIREBULLET_DIRECT_4;
		}
		else if (abs(diffX) <= FIREPIRANHAPLANT_TARGET_X)
		{
			if (diffY > 0)
				direct = FIREBULLET_DIRECT_2;
			else
				direct = FIREBULLET_DIRECT_3;
		}
		FirePlantBullet* bullet = new FirePlantBullet();
		bullet->Shoot( x + FIREPIRAHANPLANT_SHOOTING_X
			, y,nx, direct);
		Unit* unit = new Unit(grid, bullet, x + FIREPIRAHANPLANT_SHOOTING_X
			, y);
		isShooted = true;
		canShoot = false;
	}

}
void FirePiranhaPlant::GetDirect()
{
	Game* game = Game::GetInstance();
	LPSCENE scence = game->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	float mX, mY;
	mario->GetPosition(mX, mY);
	float diffX = mX - x, diffY = mY - y;
	if (diffX < 0)
		this->nx = -1;
	else
		this->nx = 1;
	if (diffY < 0)
		shootInGround = false;
	else
		shootInGround = true;
	// Nếu Mario ở trên Pipe sẽ không lao ra
	if (abs(x - mX) <= FIREPIRANHAPLANT_BBOX_WIDTH /2)
		switchTime = GetTickCount();
}

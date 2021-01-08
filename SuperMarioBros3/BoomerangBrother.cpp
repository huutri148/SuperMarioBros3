#include "BoomerangBrother.h"
#include "Pipe.h"
#include "Brick.h"
void BoomerangBrother::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BROTHER_BBOX_WIDTH;
	bottom = y + BOOMERANG_BROTHER_BBOX_HEIGHT;
}

void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	HandleTimeSwitchState();
	if (state == BOOMERANG_BROTHER_STATE_INACTIVE || isEnable == false)
		return;
	Enemy::Update(dt, coObjects);
	vy += dt * GOOMBA_GRAVITY;
	if (indexBoomerang >= weapon.size())
		indexBoomerang = 0;
	if (GetTickCount() - changeStateTime > BOOMERANG_BROTHER_CHANGE_STATE_TIME )
		this->SetState(BOOMERANG_BROTHER_STATE_WALKING);



	// Chỉnh vị trí boomerang khi đang giữ
	if (weapon[indexBoomerang]->state == BOOMERANG_STATE_HOLDING)
	{
		weapon[indexBoomerang]->x = x- BOOMERANG_BBOX_WIDTH / 2;
		if (isDead)
			weapon[indexBoomerang]->SetState(BOOMERANG_STATE_INACTIVE);
	}


	if (abs(lastStoping - x) >= BOOMERANG_BROTHER_LIMIT_MOVING)
	{
		this->vx = -vx;
		if (vx > 0)
			x = lastStoping - BOOMERANG_BROTHER_LIMIT_MOVING;
		else if (vx < 0)
			x = lastStoping + BOOMERANG_BROTHER_LIMIT_MOVING;
		lastStoping = x;

		// Nếu mario ra khỏi vùng giới hạn sẽ không ném
		Mario* mario = ((PlayScene*)Game::GetInstance()->GetCurrentScene())->GetPlayer();
		if(abs(mario->x - x) <= BOOMERANG_LIMIT_X)
			ThrowingBoomerang();
		else
			this->SetState(BOOMERANG_BROTHER_STATE_THROWING);
	}
	

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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Brick*>(e->obj) || dynamic_cast<Pipe*>(e->obj))
			{
				if (nx != 0)
				{
					this->ChangeDirect();
				}
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->nx != 0 && ny == 0)
				{
					this->ChangeDirect();
				}
			}
			else if (!dynamic_cast<Mario*>(e->obj))
			{
				x += dx;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void BoomerangBrother::Render()
{
	if (state != BOOMERANG_BROTHER_STATE_INACTIVE && isEnable == true)
	{
		int ani;
		ani = BOOMERANG_BROTHER_ANI_WALKING;
		if (isThrowing)
			ani = BOOMERANG_BROTHER_ANI_THROWING;
		animation_set->at(ani)->Render(nx, ny, round(x), round(y));
	}
	//RenderBoundingBox();
}
void BoomerangBrother::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case BOOMERANG_BROTHER_STATE_WALKING:
		isThrowing = false;
		nx = 1;
		break;
	case BOOMERANG_BROTHER_STATE_THROWING:
		changeStateTime = GetTickCount();
		break;
	case BOOMERANG_BROTHER_STATE_INACTIVE:
		x = entryX;
		y = entryY;
		break;
	case BOOMERANG_BROTHER_STATE_DEATH:
		isEnable = false;
		break;
	}
}
bool BoomerangBrother::IsDead()
{
	if (this->isDead)
	{
		return true;
	}
	return false;
}


void BoomerangBrother::SetBeingStomped()
{
	ny = 1;
	deathTime = GetTickCount();
	isDead = true;
	Game* game = Game::GetInstance();
	LPSCENE scene = game->GetCurrentScene();
	if (dynamic_cast<PlayScene*>(scene))
	{
		Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
		PointEffect* effect = new PointEffect(x, y, POINT_TYPE_1000);
		Unit* unit = new Unit(grid, effect, x, y);
	}
	deathTime = GetTickCount();
}
void BoomerangBrother::SetBeingSkilled(int nx)
{
	ny = 1;
	deathTime = GetTickCount();
	isDead = true;
	Game* game = Game::GetInstance();
	Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
	PointEffect* effect = new PointEffect(x, y, POINT_TYPE_1000);
	Unit* unit = new Unit(grid, effect, x, y);
	deathTime = GetTickCount();
}
void BoomerangBrother::HandleTimeSwitchState()
{
	if (GetTickCount64() - deathTime > GOOMA_DEATH_TIME &&
		isDead == true)
	{
		this->SetState(BOOMERANG_BROTHER_STATE_DEATH);
		return;
	}
}

void BoomerangBrother::ThrowingBoomerang()
{
	this->SetState(BOOMERANG_BROTHER_STATE_THROWING);
	if (weapon[indexBoomerang]->state == BOOMERANG_STATE_HOLDING)
	{
		isThrowing = true;
		weapon[indexBoomerang++]->Throw(x, y, nx);
	}
	else if (weapon[indexBoomerang]->state == BOOMERANG_STATE_INACTIVE)
	{
		Grid* grid = ((PlayScene*)Game::GetInstance()->GetCurrentScene())->GetGrid();
		weapon[indexBoomerang]->Hold(x, y, grid);
		
	}
}

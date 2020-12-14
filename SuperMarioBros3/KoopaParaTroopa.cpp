#include "KoopaParaTroopa.h"


void KoopaParaTroopa::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	left = x;
	top = y;
	right = x + PARATROOPA_BBOX_WIDTH;
	bottom = y + PARATROOPA_BBOX_HEIGHT;
}

void KoopaParaTroopa::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	if (state == PARATROOPA_STATE_INACTIVE)
		return;
	HandleTimeSwitchState();
	Enemy::Update(dt, coObjects);
	if (dt > 64)
		dt = 16;
	// Calculate dx, dy 
	GameObject::Update(dt);
	// fall down slower 
	if (vy > -0.2 && vy < 0.2)
			vy += 0.001f * dt;
		else
			vy += PARATROOPA_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	//DebugOut(L"\ncoEvent size: %d", coEvents.size());
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
			if (!dynamic_cast<Brick*>(e->obj) )
			{
				if (ny < 0)
				{
					vy = -PARATROOPA_JUMP_SPEED;
				}
			}
			else if(dynamic_cast<Brick*>(e->obj))
			{
				if (nx != 0)
				{
					nx = -nx;
					vx = -vx;
				}
			}

		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void KoopaParaTroopa::Render()
{
	if (state != PARATROOPA_STATE_INACTIVE)
	{
		int ani = PARATROOPA_ANI_FLAPPING;
		if (state == PARATROOPA_STATE_DEATH || isDead == true)
			ani = PARATROOPA_ANI_DEATH;
		animation_set->at(ani)->Render(nx, ny, round(x), round(y));
	}
}
void KoopaParaTroopa::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case PARATROOPA_STATE_JUMPING:
		vx = -PARAGOOMBA_WALKING_SPEED;
		nx = -1;
		vy =  0;
		break;
	case PARAGOOMBA_STATE_INACTIVE:
		x = entryX;
		y = entryY;
		break;
	case PARATROOPA_STATE_DEATH:
		vy = -GOOMBA_DIE_DEFLECT_SPEED_Y;
		vx = nx * GOOMBA_DIE_DEFLECT_SPEED_X;
		isDead = true;
		ny = 1;
		break;
	}
}
bool KoopaParaTroopa::IsDead()
{
	if (state == PARATROOPA_STATE_INACTIVE ||
		state == PARATROOPA_STATE_DEATH ||
		state == PARATROOPA_STATE_KOOPA)
		return true;
	return false;
}


void KoopaParaTroopa::SetBeingStromped()
{
	this->SetState(PARATROOPA_STATE_KOOPA);
	Game* game = Game::GetInstance();
	LPSCENE scene = game->GetCurrentScene();
	if (dynamic_cast<PlayScene*>(scene))
	{
		Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
		PointEffect* effect = new PointEffect(x, y, POINT_TYPE_100);
		Unit* unit = new Unit(grid, effect, x, y);
	}
}
void KoopaParaTroopa::SetBeingSkilled(int nx)
{
	this->nx = nx;
	this->SetState(PARATROOPA_STATE_DEATH);
	deathTime = GetTickCount();
	Game* game = Game::GetInstance();
	LPSCENE scene = game->GetCurrentScene();
	if (dynamic_cast<PlayScene*>(scene))
	{
		Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
		PointEffect* effect = new PointEffect(x, y, POINT_TYPE_200);
		Unit* unit = new Unit(grid, effect, x, y);
	}
}
void KoopaParaTroopa::HandleTimeSwitchState()
{
	DWORD current = GetTickCount();
	if (GetTickCount64() - deathTime > PARATROOPA_DEATH_TIME
		&& isDead == true)
	{
		this->isEnable = false;
		return;
	}
}
bool KoopaParaTroopa::IsInactive()
{
	if (this->state == PARATROOPA_STATE_INACTIVE)
		return true;
	return false;
}

void KoopaParaTroopa::Inactive()
{
	this->SetState(PARATROOPA_STATE_INACTIVE);
}
void KoopaParaTroopa::Active()
{
	if(kooPa->state == KOOPATROOPA_STATE_WALKING &&
		kooPa->isEnable == false)
		this->SetState(PARATROOPA_STATE_JUMPING);
	else if (kooPa->IsInactive() && kooPa->isEnable == true)
	{
		this->SetState(PARATROOPA_STATE_JUMPING);
		kooPa->isEnable = false;
	}
		
}
void KoopaParaTroopa::ChangeToKoopa(Grid* grid)
{
	kooPa->SetPosition(x, y);
	kooPa->isEnable = true;
	this->SetState(PARATROOPA_STATE_INACTIVE);
	Unit* unit = new Unit(grid, kooPa, x, y);
}
KoopaParaTroopa::KoopaParaTroopa(float x, float y) :Enemy(x, y)
{
	this->SetState(PARATROOPA_STATE_JUMPING);
	kooPa = new KoopaTroopa();
	kooPa->isEnable = false;

}
KoopaParaTroopa::KoopaParaTroopa() :Enemy()
{
	this->SetState(PARATROOPA_STATE_JUMPING);
	kooPa = new KoopaTroopa();
	kooPa->isEnable = false;
}
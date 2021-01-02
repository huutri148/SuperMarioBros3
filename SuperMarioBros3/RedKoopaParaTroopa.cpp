#include "RedKoopaParaTroopa.h"
#include"Game.h"
void RedKoopaParaTroopa::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	left = x;
	top = y;
	right = x + PARATROOPA_BBOX_WIDTH;
	bottom = y + PARATROOPA_BBOX_HEIGHT;
}

void RedKoopaParaTroopa::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	if (state == PARATROOPA_STATE_INACTIVE)
		return;
	Enemy::Update(dt, coObjects);

	HandleTimeSwitchState();
	// Calculate dx, dy 
	GameObject::Update(dt);
	if (isDead == true)
		vy += PARATROOPA_GRAVITY * dt;
	// fall down slower
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult,
			min_tx, min_ty,
			nx, ny);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void RedKoopaParaTroopa::Render()
{
	if (state != PARATROOPA_STATE_INACTIVE)
	{
		int ani = 0;
		if (state == PARATROOPA_STATE_DEATH || isDead == true)
			ani = PARATROOPA_ANI_DEATH;
		animation_set->at(ani)->Render(nx, ny, round(x), round(y));
	}
	/*RenderBoundingBox();*/
}
void RedKoopaParaTroopa::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case PARATROOPA_STATE_FLYING:
		vx = 0;
		nx = -1;
		vy = PARATROOPA_FLYING_SPEED;
		changeDirectTime = GetTickCount();
		break;
	case PARATROOPA_STATE_INACTIVE:
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
bool RedKoopaParaTroopa::IsDead()
{
	if (state == PARATROOPA_STATE_INACTIVE || state == PARATROOPA_STATE_DEATH ||
			state == PARATROOPA_STATE_KOOPA)
		return true;
	return false;
}


void RedKoopaParaTroopa::SetBeingStomped()
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
void RedKoopaParaTroopa::SetBeingSkilled(int nx)
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
bool RedKoopaParaTroopa::IsInactive()
{
	if (this->state == PARATROOPA_STATE_INACTIVE)
		return true;
	return false;
}

void RedKoopaParaTroopa::Inactive()
{
	this->SetState(PARATROOPA_STATE_INACTIVE);
}
void RedKoopaParaTroopa::Active()
{
	if (kooPa->state == KOOPATROOPA_STATE_WALKING &&
			kooPa->isEnable == false)
		this->SetState(PARATROOPA_STATE_FLYING);

	else if (kooPa->IsInactive() && kooPa->isEnable == true)
	{
		this->SetState(PARATROOPA_STATE_FLYING);
		kooPa->isEnable = false;
	}

}
void RedKoopaParaTroopa::ChangeToKoopa(Grid* grid)
{
	kooPa->SetPosition(x, y);
	kooPa->isEnable = true;
	kooPa->isAbleToActive = true;
	this->SetState(PARATROOPA_STATE_INACTIVE);
	Unit* unit = new Unit(grid, kooPa, x, y);
}
RedKoopaParaTroopa::RedKoopaParaTroopa(float x, float y, float limit) :Enemy(x,y)
{
	this->SetState(PARATROOPA_STATE_FLYING);
	kooPa = new KoopaTroopa(0);
	kooPa->isEnable = false;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_RED_PARATROOPA);
	this->SetAnimationSet(ani_set);

}
RedKoopaParaTroopa::RedKoopaParaTroopa():Enemy(x,y)
{
	Game* game = Game::GetInstance();
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_GREEN_PARATROOPA);
	this->SetAnimationSet(ani_set);
	this->SetState(PARATROOPA_STATE_FLYING);
	kooPa = new KoopaTroopa(KOOPATROOPA_RED_TYPE);
	kooPa->isEnable = false;
}
void RedKoopaParaTroopa::HandleTimeSwitchState()
{
	DWORD current = GetTickCount();
	if (current - changeDirectTime > PARATROOPA_CHANGE_DIRECT_TIME)
	{
		vy = -vy;
		changeDirectTime = current;
	}

	if (current - deathTime > PARATROOPA_DEATH_TIME
		&& isDead == true)
	{
		this->isEnable = false;
		return;
	}
}


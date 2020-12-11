#include "ParaGoomba.h"
#include "Block.h"
#include"Utils.h"
#include"Game.h"



void ParaGoomba::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	left = x;
	top = y;
	right = x + PARAGOOMBA_BBOX_WIDTH;
	if(state == PARAGOOMBA_STATE_WALKING)
		bottom = y + PARAGOOMBA_BBOX_HEIGHT_WALKING;
	else 
		bottom = y + PARAGOOMBA_BBOX_HEIGHT_FLAPPING;
}

void ParaGoomba::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"\nState: %d", state);
	if (state == PARAGOOMBA_STATE_INACTIVE)
		return;
/* Hoàn thành cơ bản về di chuyển của ParaKoopa
* nhưng cần bổ sung thêm về hướng di chuyển (Chasing Mario)
* tạo và truyền thông tin chung của Mario cho mọi Enemy
*/
	HandleTimeSwitchState();
	Enemy::Update(dt, coObjects);
	if (vy > -0.2 && vy < 0.2)
		vy += MARIO_LOWER_GRAVITY * dt;
	else
		vy += PARAGOOMBA_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

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
			if (!dynamic_cast<Block*>(e->obj) && !dynamic_cast<Goomba*>(e->obj))
			{
				if (nx != 0 && ny == 0)
				{
					this->ChangeDirect();
				}
				else if (ny < 0 )
				{
					if(state == PARAGOOMBA_STATE_SUPER_JUMPING)
						this->SetState(PARAGOOMBA_STATE_WALKING);
					if (state == PARAGOOMBA_STATE_FLAPPING)
						this->SetState(PARAGOOMBA_STATE_FLAPPING);
				}
				
			}
			else
			{
				x += dx;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void ParaGoomba::Render()
{
	if (state != PARAGOOMBA_STATE_INACTIVE)
	{
		int ani = 0;
		if (state == PARAGOOMBA_STATE_WALKING)
			ani = PARAGOOMBA_ANI_WALKING;
		else
			ani = PARAGOOMBA_ANI_JUMPING;
		animation_set->at(ani)->Render(nx, ny, round(x), round(y));
	}
}
void ParaGoomba::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case PARAGOOMBA_STATE_WALKING:
		vx =- PARAGOOMBA_WALKING_SPEED;
		nx = -1;
		vy = 0;
		break;
	case PARAGOOMBA_STATE_FLAPPING:
		vy = -PARAGOOMBA_JUMP_SPEED;
		break;
	case PARAGOOMBA_STATE_SUPER_JUMPING:
		vy = -PARAGOOMBA_SUPER_JUMP_SPEED;
		break;
	case PARAGOOMBA_STATE_INACTIVE:
		x = entryX;
		y = entryY;
		break;
	case PARAGOOMBA_STATE_DEATH:
		vy = -GOOMBA_DIE_DEFLECT_SPEED_Y;
		vx = nx * GOOMBA_DIE_DEFLECT_SPEED_X;
		isDead = true;
		ny = 1;
		break;

	}
}
bool ParaGoomba::IsDead()
{
	if(state == PARAGOOMBA_STATE_INACTIVE ||
		state == PARAGOOMBA_STATE_DEATH ||
		state == PARAGOOMBA_STATE_GOOMBA)
		return true;
	return false;
}


void ParaGoomba::SetBeingStromped()
{
	this->SetState(PARAGOOMBA_STATE_GOOMBA);
	Game* game = Game::GetInstance();
	LPSCENE scene = game->GetCurrentScene();
	if (dynamic_cast<PlayScene*>(scene))
	{
		Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
		PointEffect* effect = new PointEffect(x, y, POINT_TYPE_100);
		Unit* unit = new Unit(grid, effect, x, y);
	}
}
void ParaGoomba::SetBeingSkilled(int nx)
{
	this->nx = nx;
	this->SetState(PARAGOOMBA_STATE_DEATH);
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
void ParaGoomba::HandleTimeSwitchState()
{
	DWORD current = GetTickCount();
	if (GetTickCount64() - deathTime > GOOMA_DEATH_TIME 
		 && isDead == true)
	{
		this->isEnable = false;
		return;
	}
	else if (current - switchStateTime > PARAGOOMBA_SWITCH_STATE_TIME)
	{
		if (state == PARAGOOMBA_STATE_WALKING)
		{
			y -= PARAGOOMBA_BBOX_HEIGHT_FLAPPING - PARAGOOMBA_STATE_WALKING;
			this->SetState(PARAGOOMBA_STATE_FLAPPING);
		}
			
		else if (state == PARAGOOMBA_STATE_FLAPPING)
			this->SetState(PARAGOOMBA_STATE_SUPER_JUMPING);
		switchStateTime = GetTickCount();
	}
}
bool ParaGoomba::IsInactive()
{
	if (this->state == PARAGOOMBA_STATE_INACTIVE)
		return true;
	return false;
}

void ParaGoomba::Inactive()
{
	this->SetState(PARAGOOMBA_STATE_INACTIVE);
}
void ParaGoomba::Active()
{
	if (goomba->state == GOOMBA_STATE_WALKING && goomba->isEnable == false)
	{
		this->SetState(PARAGOOMBA_STATE_WALKING);
	}
	else if (goomba->IsInactive() && goomba->isEnable == true)
	{
		this->SetState(PARAGOOMBA_STATE_WALKING);
		goomba->isEnable = false;
	}
}
void ParaGoomba::ChangeToGoomba(Grid* grid)
{
	goomba->SetPosition(x, y);
	goomba->isEnable = true;
	this->SetState(PARAGOOMBA_STATE_INACTIVE);
	Unit* unit = new Unit(grid, goomba, x, y);
}

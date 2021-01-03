#include "PiranhaPlant.h"
#include "Ground.h"
#include "Brick.h"
#include "Utils.h"
void PiranhaPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
}
void PiranhaPlant::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	HandleTimeSwitchState();
	if (state == PIRANHAPLANT_STATE_INACTIVE)
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
		if (lastStateY - y > 32)
		{
			y = lastStateY - 32;
			lastStateY = y;
			isOutOfPipe = true;
			this->SetState(PIRANHAPLANT_STATE_BITING);
			switchTime = GetTickCount();
		}
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
					this->SetState(PIRANHAPLANT_STATE_BITING);
					switchTime = GetTickCount();
					this->y = y0 + e->t * dy + e->ny * 0.4f;
					lastStateY = y;
				}
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void PiranhaPlant::Render()
{
	if (state != PIRANHAPLANT_STATE_INACTIVE)
	{
		int ani = 0;
		if (state == PIRANHAPLANT_STATE_DARTING)
		{
			if (type == PIRANHAPLANT_GREEN_TYPE)
				ani = PIRANHAPLANT_GREEN_ANI_DARTING;
			else
				ani = PIRANHAPLANT_RED_ANI_DARTING;
		}	
		else if (state == PIRANHAPLANT_STATE_BITING)
		{
			if (type == PIRANHAPLANT_GREEN_TYPE)
				ani = PIRANHAPLANT_GREEN_ANI_BITING;
			else
				ani = PIRANHAPLANT_RED_ANI_BITING;
		}
		else if (state == PIRANHAPLANT_STATE_DEATH)
		{
			ani = PIRANHAPLANT_ANI_DEATH;
		}
		animation_set->at(ani)->Render(-1,round( x),round( y));
	}
	//RenderBoundingBox();
}
PiranhaPlant::PiranhaPlant(float x, float y, int _type) :Enemy(x, y)
{
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_PIRAHAPLANT);
	this->SetAnimationSet(ani_set);
	this->type = _type;
	this->state = PIRANHAPLANT_STATE_DARTING;
	vy = PIRANHAPLANT_DARTING_SPEED;
	this->isOutOfPipe = true;
	switchTime = 0;
}
void PiranhaPlant::SetState(int _state)
{
	Enemy::SetState(_state);
	switch (_state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		if (isOutOfPipe == false)
		{
			vy = -PIRANHAPLANT_DARTING_SPEED;
		}
		else
			vy = PIRANHAPLANT_DARTING_SPEED;
		break;
	case PIRANHAPLANT_STATE_BITING:
		vy = 0;
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		x = entryX;
		y = entryY;
		break;
	}
}
bool PiranhaPlant::IsDead()
{
	if (isDead == true)
	{
		return true;
	}
	return false;
}
void PiranhaPlant::SetBeingSkilled(int nx)
{
	this->SetState(PIRANHAPLANT_STATE_DEATH);
	y += PIRANHAPLANT_BBOX_HEIGHT -
		PIRANHAPLANT_BBOX_DEATH_HEIGHT;
	deathTime = GetTickCount();
	isDead = true;
}
void PiranhaPlant::SetBeingStomped()
{

}
void PiranhaPlant::HandleTimeSwitchState()
{
	Game* game = Game::GetInstance();
	LPSCENE scence = game->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	float mX, mY;
	mario->GetPosition(mX, mY);
	float diffX = mX - x, diffY = mY - y;
	if (state == PIRANHAPLANT_STATE_BITING &&
		GetTickCount() - switchTime > PIRANHAPLANT_SWITCHING_STATE_TIME
		&& switchTime != 0)
	{
		switchTime = 0;
		this->SetState(PIRANHAPLANT_STATE_DARTING);
	}
	if (state == PIRANHAPLANT_STATE_DEATH &&
		GetTickCount() - deathTime > PIRANHAPLANT_INACTIVE_TIME
		&& deathTime != 0)
	{
		isEnable = false;
		deathTime = 0;
	}
	if (abs(x - mX) <= FIREPIRANHAPLANT_BBOX_WIDTH * 2 && isOutOfPipe == false)
		switchTime = GetTickCount();
}

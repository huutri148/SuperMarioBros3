#include "MovingPlattform.h"
#include"Game.h"
void MovingPlattform::GetBoundingBox(float& left, float& top,
	float& right, float& bottom, bool isEnable)
{
	left = x;
	top = y;
	right = x + MOVING_PLATTFORM_BBOX_WIDTH;
	bottom = y + MOVING_PLATTFORM_BBOX_HEIGHT;
}

void MovingPlattform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MOVING_PLATTFORM_STATE_INACTIVE || isEnable == false)
		return;

	GameObject::Update(dt, coObjects);


	if (this->state == MOVING_PLATTFORM_STATE_FALLING)
		vy += dt * MOVING_PLATTFORM_GRAVITY;


	if (isBeingTouched)
	{
		PlayScene* playScene = (PlayScene*)Game::GetInstance()->GetCurrentScene();
		Mario* mario = playScene->GetPlayer();
		if (mario->isTouchingPlattform)
		{
			if(mario->x >= x - MARIO_SMALL_BBOX_WIDTH && 
				mario->x < x + MOVING_PLATTFORM_BBOX_WIDTH)
				mario->SetY(this->y);
			else
			{
				mario->isTouchingPlattform = false;
				isBeingTouched = false;
				mario->vy = 0;
			}
		}
		else
		{
			isBeingTouched = false;
		}
	}
	
	
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Mario*>(e->obj))
			{
				if (nx != 0 && ny == 0)
				{
					
					e->obj->x -= (min_tx * dx + nx * 0.4f);
	
				}
				else if (e->ny < 0)
				{
					y += dy;
				}
			}
		}
	}

}
void MovingPlattform::Render()
{
	if (state == MOVING_PLATTFORM_STATE_INACTIVE)
		return;

	int ani = MOVING_PLAT_FORM_ANI;

	animation_set->at(ani)->Render(nx, ny, round(x), round(y));
	//RenderBoundingBox();
}



void MovingPlattform::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MOVING_PLATTFORM_STATE_MOVING:
		vx = - MOVING_PLATTFORM_SPEED_X;
		vy = 0;
		break;
	case MOVING_PLATTFORM_STATE_FALLING:
		vx = 0;
		break;
	case MOVING_PLATTFORM_STATE_INACTIVE:
		x = entryX;
		y = entryY;
		isBeingTouched = false;
		break;
	}
}




MovingPlattform::MovingPlattform(float entryX, float entryY)
{

	isAbleToActive = true;

	this->entryX = entryX;
	this->entryY = entryY;
	this->x = entryX;
	this->y = entryY;


	this->SetState(MOVING_PLATTFORM_STATE_MOVING);

	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_MOVING_PLATTFORM);
	this->SetAnimationSet(ani_set);
}




void MovingPlattform::AbleToActive()
{
	Game* game = Game::GetInstance();
	float cam_x = game->GetCamX();
	float cam_y = game->GetCamY();
	if ((this->entryX <= cam_x || this->entryX > cam_x + game->GetScreenWidth()))
		this->isAbleToActive = true;
}

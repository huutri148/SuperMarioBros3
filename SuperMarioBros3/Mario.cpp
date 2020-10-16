#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->ny < 0)
			{
				if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				
					
				}
			else if (e->nx != 0)
			{
				// if is enimies
				if (untouchable == 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						if (form > MARIO_SMALL_FORM)
						{
							form -= 1;
							StartUntouchable();
						}
						else
							SetState(MARIO_STATE_DIE);
					}
				}
			}

			}
			
		}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


	


void CMario::Render()
{
	int ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
		if (form == MARIO_BIG_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		else if (form == MARIO_SMALL_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
		else if (form == MARIO_FIRE_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
		else if (form == MARIO_FIRE_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
		else if (form == MARIO_RACCOON_FORM)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
				else ani = MARIO_ANI_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_WALKING_LEFT;
		}


	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (form == MARIO_BIG_FORM)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (form == MARIO_SMALL_FORM)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (form == MARIO_FIRE_FORM)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
	else if (form == MARIO_RACCOON_FORM)
	{
		right = x + MARIO_RACCOON_BBOX_WIDTH;
		bottom = y + MARIO_RACCOON_BBOX_HEIGHT;
	}
}

void CMario::UpForm()
{
	form = MARIO_RACCOON_FORM;
	}


#include "Portal.h"
#include"Game.h"
void Portal::Render()
{
	if (state != PORTAL_STATE_CONGRATULATE)
	{
		int ani = 0;
		if (state == PORTAL_STATE_ROULETTE)
		{
			ani = PORTAL_ANI_ROULETTE;
		}
		else if (state == PORTAL_STATE_PICKED)
		{
			if (idCard == CARD_MUSHROOM)
				ani = PORTAL_ANI_PICKED_MUSHROOM;
			else if (idCard == CARD_STARMAN)
				ani = PORTAL_ANI_PICKED_STARMAN;
			else
				ani = PORTAL_ANI_PICKED_FIREFLOWER;
		}
		animation_set->at(ani)->Render(-1, round(x), round(y));
	}
	else if(state == PORTAL_STATE_CONGRATULATE)
	{
		float  cam_y = (Game::GetInstance())->GetCamY();
		float  cam_x = (Game::GetInstance())->GetCamX();
		congratulate->Draw(-1,cam_x + 16,cam_y + 32 );
		card->Draw(-1, cam_x + 189, cam_y + 74);
	}
}

void Portal::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case PORTAL_STATE_ROULETTE:
		vx = 0;
		vy = 0;
		idCard = CARD_MUSHROOM;
		changeTime = GetTickCount();
		break;
	case PORTAL_STATE_PICKED:
		vy = -PORTAL_PICKED_SPEED;
		break;
	case PORTAL_STATE_CONGRATULATE:
		vy = 0;
		break;
	case PORTAL_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}
void Portal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == PORTAL_STATE_ROULETTE)
	{
		if (GetTickCount() - changeTime > PORTAL_ROULETTE_CHANGE_TIME)
		{
			if (idCard == CARD_MUSHROOM)
				idCard = CARD_STARMAN;
			else if (idCard == CARD_STARMAN)
				idCard = CARD_FIREFLOWER;
			else
				idCard = CARD_MUSHROOM;
		}
	}
	if (state == PORTAL_STATE_PICKED)
	{
		float  cam_y = (Game::GetInstance())->GetCamY();
		if (y < cam_y)
		{
			Sprites* sprite = Sprites::GetInstance();
			this->SetState(PORTAL_STATE_CONGRATULATE);
			if (idCard == CARD_MUSHROOM)
				card = sprite->Get(SPRITE_CARD_MUSHROOM);
			else if (idCard == CARD_STARMAN)
				card = sprite->Get(SPRITE_CARD_STARMAN);
			else
				card = sprite->Get(SPRITE_CARD_FLOWER);
		}
			
	}
	GameObject::Update(dt, coObjects);
	y += dy;
}

void Portal::GetBoundingBox(float& left, float& top,
	float& right, float& bottom, bool isEnable)
{
	if (state == PORTAL_STATE_ROULETTE)
	{
		left = x;
		top = y;
		right = left + PORTAL_BBOX_WIDTH;
		bottom = top + PORTAL_BBOX_HEIGHT;
	}
}
int Portal::GetPortal()
{
	this->SetState(PORTAL_STATE_PICKED);
	return idCard;
}

Portal::Portal()
{
	Sprites* sprites = Sprites::GetInstance();
	this->SetState(PORTAL_STATE_ROULETTE);
	congratulate = sprites->Get(SPRITE_CONGRATULATE);
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_PORTAL_ID);
	this->SetAnimationSet(ani_set);
}

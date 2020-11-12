#include "Coin.h"
#include <algorithm>
#include "Mario.h"
#include"Brick.h"
void Coin::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	if (isEnable == true)
	{

		left = x;
		top = y;
		right = x + COIN_BBOX_WIDTH;
		bottom = y + COIN_BBOX_HEIGHT;
	}

	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}

void Coin::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{

}
void Coin::Render()
{
	if (isEnable == true)
	{
		if (state != COIN_STATE_INACTIVE)
		{
			int ani;
			ani = COIN_ANI_TYPE_2;
			animation_set->at(ani)->Render(nx, x, y);
		}
	}
	//RenderBoundingBox();
}
void Coin::SetState(int state)
{
	switch (state)
	{
	case COIN_STATE_ACTIVE:
		isEnable = true;
		vx = 0;
		vy = 0;
		nx = -1;
		break;
	case COIN_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}
void Coin::Disappearance()
{
	this->SetState(COIN_STATE_INACTIVE);
}

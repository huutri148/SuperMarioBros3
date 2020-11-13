#include "RaccoonLeaf.h"
#include"Game.h"
#include"Mario.h"
void RaccoonLeaf::Render()
{
	
	if (isEnable == true)
	{
		int ani = RACCOONLEAF_ANI_NORMAL;
		animation_set->at(ani)->Render(this->nx, x, y);
	}
	//RenderBoundingBox();
}

void RaccoonLeaf::GetBoundingBox(float& l, float& t, float& r,
	float& b, bool isEnable)
{
	if (isEnable == true)
	{
		l = x;
		t = y;
		r = x + RACCOONLEAF_BBOX_WIDTH;
		b = y + RACCOONLEAF_BBOX_HEIGHT;
	}
	else
	{
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
	
}
void RaccoonLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != RACCOONLEAF_STATE_APPEARANCE)
		return;
	GameObject::Update(dt, coObjects);
	if(vy <= 0)
		vy += RACCOONLEAF_GRAVITY * dt;
	x += dx;
	y += dy;
	
	if (vy > 0 && vx == 0)
	{
		vy = RACCOONLEAF_SPEED_FALL;
		vx = RACCOONLEAF_SPEED_X;
		changeDirectTime = GetTickCount();
	}
	else 
	{
		DWORD current = GetTickCount();
		if (current - changeDirectTime >
			RACCOONLEAF_CHANGE_DIRECT_TIME)
		{
			vx = -vx;
			this->nx = -this->nx;
			changeDirectTime = GetTickCount();
		}
		
	}
		

}
void RaccoonLeaf::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case RACCOONLEAF_STATE_APPEARANCE:
		isEnable = true;
		nx = 1;
		vx = 0;
		vy = - RACCOONLEAF_SPEED_APPEARANCE_Y;
		break;
	case RACCOONLEAF_STATE_INACTIVE:
		isEnable = false;
		vy = 0;
		vx = 0;
		break;
	}
}
void RaccoonLeaf::Appear(float x, float y)
{
	this->SetPosition(x, y);
	this->SetState(RACCOONLEAF_STATE_APPEARANCE);
	this->isEnable = true;
}
void RaccoonLeaf::Used()
{
	
	LPSCENE scence = Game::GetInstance()->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	mario->TurnRaccoonForm();
	this->SetState(RACCOONLEAF_STATE_INACTIVE);
}
RaccoonLeaf::RaccoonLeaf()
{
}

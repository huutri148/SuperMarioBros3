#include "Boomerang.h"
#include "Utils.h"


void Boomerang::Render()
{
	if (state != BOOMERANG_STATE_INACTIVE)
	{
		int ani = BOOMERANG_ANI_HOLDING;
		if (state == BOOMERANG_STATE_THROWING)
			ani = BOOMERANG_ANI_THROWING;
		animation_set->at(ani)->Render(nx ,ny, round(x), round(y));
	}
		
}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOOMERANG_STATE_INACTIVE)
		return;
	GameObject::Update(dt, coObjects);
	if (state == BOOMERANG_STATE_THROWING)
	{
		if (-y + startY >= BOOMERANG_LIMIT_Y && vy < 0)
		{
			vy = 0;
			vx = BOOMERANG_SPEED_X;
		}
		if (x - startX >= 0 && vx > 0)
		{
			vy = BOOMERANG_SPEED_Y;
			vx = 0;
			startY = y;
		}
		if (y - startY >= BOOMERANG_LIMIT_Y && vy > 0)
		{
			vy = 0;
			vx = -BOOMERANG_SPEED_X;
			startX = x - BOOMERANG_LIMIT_X;
		}
		if (startX - x >= 0 && vx < 0)
		{
			this->SetState(BOOMERANG_STATE_INACTIVE);
		}
		x += dx;
		y += dy;
	}
}
void Boomerang::GetBoundingBox(float& l, float& t,
	float& r, float& b, bool isEnable)
{
	if (state == BOOMERANG_STATE_THROWING)
	{
		l = x;
		t = y;
		r = x + BOOMERANG_BBOX_WIDTH;
		b = y + BOOMERANG_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
	
}
void Boomerang::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case BOOMERANG_STATE_THROWING:
		vy = -BOOMERANG_SPEED_Y;
		vx = 0;
		startX = x + BOOMERANG_LIMIT_X;
		startY = y ;
		break;
	case BOOMERANG_STATE_INACTIVE:
		isEnable = false;
	case BOOMERANG_STATE_HOLDING:
		vx = 0;
		vy = 0;
		isEnable = true;
		break;
	}
}
void Boomerang::Throw(float x, float y,int nx)
{
	
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->SetState(BOOMERANG_STATE_THROWING);
}
void Boomerang::Hold(float x, float y, Grid* grid)
{
	this->x = x - BOOMERANG_BBOX_WIDTH/2;
	this->y = y - 6;
	nx = 1;
	this->SetState(BOOMERANG_STATE_HOLDING);
	if (unit == NULL)
	{
		unit = new Unit(grid, this, x, y);
	}
	else
		unit->Move(x, y);
}
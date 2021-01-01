#include "Boomerang.h"


void Boomerang::Render()
{
	if(state == BOOMERANG_STATE_THROWING)
		animation_set->at(BOOMERANG_ANI_THROWING)->Render(-1, round(x), round(y));
}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOOMERANG_STATE_INACTIVE)
		return;
	if (dt > 64)
		dt = 32;
	GameObject::Update(dt, coObjects);
	vy += dt * 0.00005f;
	if (vy >= 0 && vx * nx > 0)
		vx = -vx;
	if ((x - startX < 0 && nx > 0) || (x - startX > 0 && nx < 0))
	{
		this->SetState(BOOMERANG_STATE_INACTIVE);
	}
	x += dx;
	y += dy;
}
void Boomerang::GetBoundingBox(float& l, float& t,
	float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + BOOMERANG_BBOX_WIDTH;
	b = y + BOOMERANG_BBOX_HEIGHT;
}
void Boomerang::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case BOOMERANG_STATE_THROWING:
		vx = nx * 0.08f;
		vy = - 0.05f;
		isEnable = true;
		startX = x;
		startY = y;
		break;
	case BOOMERANG_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}
void Boomerang::Throw(float x, float y,int nx,Grid* grid)
{
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->SetState(BOOMERANG_STATE_THROWING);
	if (unit == NULL)
	{
		unit = new Unit(grid, this, x, y);
	}
	else
		unit->Move(x, y);
}

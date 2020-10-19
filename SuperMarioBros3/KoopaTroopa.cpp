#include "KoopaTroopa.h"
#include "debug.h"


void CKoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPATROOPA_BBOX_WIDTH;

	if (state == KOOPATROOPA_STATE_HIDING)
		bottom = y + KOOPATROOPA_BBOX_HEIGHT_HIDING;
	else
		bottom = y + KOOPATROOPA_BBOX_HEIGHT;
}
void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
		
	vy += KOOPATROOPA_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != KOOPATROOPA_STATE_HIDING)
		CalcPotentialCollisions(coObjects, coEvents);
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

		if (nx != 0) vx = -KOOPATROOPA_WALKING_SPEED;
		if (ny != 0) vy = 0;
	}
	DebugOut(L"KoopaTroopa Position: X: %f,y: %f", this->x, this->y);
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//Enemy::Update(dt, coObjects);
	
}
void CKoopaTroopa::Render()
{
	int ani;
	if (state == KOOPATROOPA_STATE_WALKING)
		ani = KOOPATROOPA_ANI_WALKING;
	else
		ani = KOOPATROOPA_ANI_HIDING;
	animations[ani]->Render(x, y);
}
void CKoopaTroopa::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case KOOPATROOPA_STATE_WALKING:
		vx = -KOOPATROOPA_WALKING_SPEED;
		nx = -1;
		break;
	case KOOPATROOPA_STATE_HIDING:
		y += 10;
		nx = 0;
		vx = 0;
		vy = 0;
		break;
	}
}

void CKoopaTroopa::SetDie()
{
	this->SetState(KOOPATROOPA_STATE_HIDING);
}

bool CKoopaTroopa::IsDead()
{
	if (this->state == KOOPATROOPA_STATE_HIDING)
	{
		return true;
	}
	return false;
}

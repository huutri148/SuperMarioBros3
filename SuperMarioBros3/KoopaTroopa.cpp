#include "KoopaTroopa.h"


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
	if (this->state == KOOPATROOPA_STATE_HIDING)
	{
		return;
	}
	Enemy::Update(dt, coObjects);
	x += dx;
	y += dy;
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
		break;
	case KOOPATROOPA_ANI_HIDING:
		y += KOOPATROOPA_BBOX_HEIGHT_HIDING + 1;
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

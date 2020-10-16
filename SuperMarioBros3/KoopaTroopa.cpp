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
	CGameObject::Update(dt, coObjects);
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
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPATROOPA_STATE_WALKING:
		vx = -KOOPATROOPA_WALKING_SPEED;
		break;
	case KOOPATROOPA_ANI_HIDING:
		vx = 0;
		vy = 0;
		break;
	}
}

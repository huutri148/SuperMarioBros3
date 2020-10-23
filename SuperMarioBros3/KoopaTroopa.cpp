#include "KoopaTroopa.h"
#include "debug.h"
#include "Brick.h"


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

	//Xét nếu đang bị cầm ở dạng shell
	if (isPickedUp == true)
	{
		if (mario->isPickingUp == true)
		{
			this->vx = mario->vx;
			this->vy = mario->vy;
			this->nx = mario->nx;
			this->ny = mario->ny;
		}
		else // nếu người chơi nhả nút giữ sẽ trở về Hiding
		{
			isPickedUp = false;
			this->SetState(KOOPATROOPA_STATE_HIDING);
		}
	
	}
		vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if(state != KOOPATROOPA_STATE_DIE_NX)
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
		if (nx != 0)
		{
		}
		if (ny < 0) vy = 0;
		if (state == KOOPATROOPA_STATE_HIDING )
		{
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<Enemy*>(e->obj))
				{
					Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
					if (e->nx != 0)
					{
						enemy->SetDie(true);
						this->SetState(KOOPATROOPA_STATE_DIE_NX);
					}
				}
			}
		}
	}
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
		vy = 0;
		vx = 0;
		break;
	case KOOPATROOPA_STATE_DIE_NX:
		vy = -KOOPATROOPA_DIE_DEFLECT_SPEED;
		vx = 0;
		break;

	}
}

void CKoopaTroopa::SetDie(bool n)
{
	// true: nx. false = ny
	if (n == true)
		this->SetState(KOOPATROOPA_STATE_DIE_NX);
	else
		this->SetState(KOOPATROOPA_STATE_HIDING);
}

bool CKoopaTroopa::IsDead()
{
	if ( this->state == KOOPATROOPA_STATE_HIDING)
	{
		return true;
	}
	return false;
}
void CKoopaTroopa::IsKicked(int nx)
{
	this->nx = nx;
	if (this->nx < 0)
	{
		this->vx = -KOOPATROOPA_BUMP_SPEED;
	}
	else
		this->vx = KOOPATROOPA_BUMP_SPEED;
}
bool CKoopaTroopa::IsHiding()
{
	if (this->state == KOOPATROOPA_STATE_HIDING)
		return true;
	return false;
}

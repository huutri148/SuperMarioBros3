#include "Mushroom.h"
#include"Game.h"
void Mushroom::Render()
{
	int ani = MUSHROOM_ANI_POWERUP;
	if (type == MUSHROOM_TYPE_1UP)
		ani = MUSHROOM_ANI_1UP;
	animation_set->at(ani)->Render(this->nx, round(x),round( y));
	//RenderBoundingBox();
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r,
	float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + MUSHROOM_BBOX_WIDTH;
	b = y + MUSHROOM_BBOX_HEIGHT;

}
void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MUSHROOM_STATE_INACTIVE)
		return;
	GameObject::Update(dt, coObjects);
	if (state == MUSHROOM_STATE_WALKING || y <= appearY)
		vy += RACCOONLEAF_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult,
			min_tx, min_ty,
			nx, ny);
		this->y += min_ty * dy + ny * 0.4f;
		this->x += min_tx * dx + nx * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->ny < 0)
				{
					this->SetState(MUSHROOM_STATE_WALKING);
					vy = 0;
				}
				if (nx != 0 && ny == 0)
				{
					if (dynamic_cast<Pipe*>(e->obj) ||
						dynamic_cast<Brick*>(e->obj))
					{
						this->vx = -this->vx;
						this->nx = -this->nx;
					}
					else
					{
						x += dx;
					}
				}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Mushroom::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case MUSHROOM_STATE_APPEARANCE:
		isEnable = true;
		vx = 0;
		vy = -MUSHROOM_SPEED_APPEARANCE_Y;
		break;
	case MUSHROOM_STATE_INACTIVE:
		isEnable = false;
		vy = 0;
		vx = 0;
		break;
	case MUSHROOM_STATE_WALKING:
		vx = -nx * MUSHROOM_SPEED_X;
		break;
	}
}
//void Mushroom::Appear(float x, float y, int _type)
//{
//	appearY = y - MUSHROOM_BBOX_HEIGHT - 1;
//	this->type = _type;
//	this->SetAppearedDirect();
//	this->SetPosition(x, y);
//	this->SetState(MUSHROOM_STATE_APPEARANCE);
//	this->isEnable = true;
//}
void Mushroom::Appear(float x, float y)
{
	this->SetPosition(x, y);
	appearY = y - MUSHROOM_BBOX_HEIGHT - 1;
	this->SetAppearedDirect();
	this->SetState(MUSHROOM_STATE_APPEARANCE);
}
void Mushroom::Used()
{
	this->SetState(MUSHROOM_STATE_INACTIVE);
	LPSCENE scence = Game::GetInstance()->GetCurrentScene();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	Grid* grid = ((PlayScene*)scence)->GetGrid();

	if (type == MUSHROOM_TYPE_POWERUP)
	{
		mario->TurnBigForm();
		PointEffect* effect = new PointEffect(x, y, POINT_TYPE_1000);
		Unit* unit = new Unit(grid, effect, x, y);
		mario->GainPoint(1000);
	}
	else
	{
		mario->GainLife();
		PointEffect* effect = new PointEffect(x, y, POINT_TYPE_1UP);
		Unit* unit = new Unit(grid, effect, x, y);
	}
	

}
Mushroom::Mushroom(int type)
{
	this->type = type;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(72);
	this->SetAnimationSet(ani_set);
}
void Mushroom::SetAppearedDirect()
{
	PlayScene* scene = (PlayScene*)Game::GetInstance()->GetCurrentScene();
	Mario* mario = scene->GetPlayer();
	this->nx = mario->nx;
}

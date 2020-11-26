#include "FirePlantBullet.h"
#include "Utils.h"
void FirePlantBullet::Render()
{
	animation_set->at(0)->Render(-1, round(x), round(y));
}
void FirePlantBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}
void FirePlantBullet::GetBoundingBox(float& l, float& t, 
	float& r, float& b, bool isEnable)
{
	if (isEnable == true)
	{
		l = x;
		t = y;
		r = x + FIREBULLET_BBOX_WIDTH;
		b = y + FIREBULLET_BBOX_HEIGHT;
	}
	else
	{
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
}
void FirePlantBullet::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case FIREBULLET_DIRECT_1:
		vx = nx * FIREBULLET_SPEED_NX;
		vy = FIREBULLET_SPEED_NY_1;
		break;
	case FIREBULLET_DIRECT_2:
		vx = nx * FIREBULLET_SPEED_NX;
		vy = FIREBULLET_SPEED_NY_2;
		break;
	case FIREBULLET_DIRECT_3:
		vx = nx * FIREBULLET_SPEED_NX;
		vy =- FIREBULLET_SPEED_NY_2;
		break;
	case FIREBULLET_DIRECT_4:
		vx = nx * FIREBULLET_SPEED_NX;
		vy =- FIREBULLET_SPEED_NY_1;
		break;
	}
}

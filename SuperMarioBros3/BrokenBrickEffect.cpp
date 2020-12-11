#include "BrokenBrickEffect.h"
BrokenBrickEffect::BrokenBrickEffect(float x, float y)
{
	this->x = x;
	this->y = y;
	x1 = x2 = x;
	y1 = y2 = y;
	Sprites* sprites = Sprites::GetInstance();
	for (unsigned int i = 0; i < 4; i++)
	{
		brokenBrickSprite.push_back(sprites->Get(SPRITE_BROKENBRICK_EFFECT_ID));
	}
	this->isEnable = true;
	this->SetState(BROKENBRICK_EFFECT_STATE_APPEAR);
	
}
void BrokenBrickEffect::Render()
{
	if (state != BROKENBRICK_EFFECT_STATE_APPEAR)
		return;
	//animation_set->at(0)->Render(-1, round(x), round(y));
	brokenBrickSprite.at(0)->Draw(-1, x1, y1);
	brokenBrickSprite.at(1)->Draw(-1, x1, y2);
	brokenBrickSprite.at(2)->Draw(-1, x2, y1);
	brokenBrickSprite.at(3)->Draw(-1, x2, y2);
}

void BrokenBrickEffect::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case BROKENBRICK_EFFECT_STATE_APPEAR:
		vx = BROKENBRICK_EFFECT_SPEED_X;
		vy1 = -BROKENBRICK_EFFECT_SPEED_Y1;
		vy2 = -BROKENBRICK_EFFECT_SPEED_Y2;
		break;
	case BROKENBRICK_EFFECT_STATE_INACTIVE:
		isEnable = false;
		break;
	}
}
void BrokenBrickEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BROKENBRICK_EFFECT_STATE_INACTIVE)
		return;
	GameObject::Update(dt);
	float dy1, dy2;
	dy1 = vy1 * dt;
	dy2 = vy2 * dt;
	x1 += dx;
	x2 +=-dx;
	y1 += dy1;
	y2 += dy2;
	vy1 += dt*GOOMBA_GRAVITY;
	vy2 += dt * GOOMBA_GRAVITY;
	if (GetTickCount() - appearTime > BROKENBRICK_EFFECT_APPEAR_TIME
		&& appearTime != 0)
		this->SetState(BROKENBRICK_EFFECT_STATE_INACTIVE);
}

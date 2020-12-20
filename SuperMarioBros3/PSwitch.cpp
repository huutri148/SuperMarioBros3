#include "PSwitch.h"
#include"Game.h"
#include"Mario.h"

void PSwitch::Render()
{

	int ani = PSWITCH_ANI_APPEAR;
	if (state == PSWITCH_STATE_PRESSED)
		ani = PSWITCH_ANI_PRESSED;
	animation_set->at(ani)->Render(-1, round(x),round( y));

}

void PSwitch::GetBoundingBox(float& l, float& t, float& r,
	float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + PSWITCH_BBOX_WIDTH;
	if (state == PSWITCH_STATE_PRESSED)
		b = y + PSWITCH_PRESSED_BBOX_HEIGHT;
	else
		b = y + PSWITCH_BBOX_HEIGHT;
}
void PSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	bool _isTransForm = Brick::isTransForm;
	if (isEnable == false || _isTransForm == false)
		return;
	if (GetTickCount() - appearTime > PSWITCH_TRANSFORM_TIME
		&& appearTime != 0)
	{
		 Brick::isTransForm= false;
		 appearTime = 0;
	}
	
}
void PSwitch::SetState(int _state)
{
	GameObject::SetState(_state);
	switch (_state)
	{
	case PSWITCH_STATE_APPEAR:
		vy = 0;
		isEnable = true;
		break;
	case PSWITCH_STATE_PRESSED:
		y += PSWITCH_BBOX_HEIGHT - PSWITCH_PRESSED_BBOX_HEIGHT;
		appearTime = GetTickCount();
		break;
	case PSWITCH_STATE_INACTIVE:
		isEnable = false;
		vx = 0;
		vy = 0;
		break;
	}
}
void PSwitch::Appear(float x, float y)
{
	this->SetPosition(x,
		y - PSWITCH_BBOX_HEIGHT  + 2);
	this->SetState(PSWITCH_STATE_APPEAR);
}
void PSwitch::Used()
{
	if (state != PSWITCH_STATE_PRESSED)
	{
		this->SetState(PSWITCH_STATE_PRESSED);
		Brick::isTransForm = true;
	}
}
PSwitch::PSwitch()
{
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_PSWITCH);
	this->SetAnimationSet(ani_set);
}

#include "PiranhaPlant.h"
void PiranhaPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom,
	bool isEnable)
{
	if (isEnable == true)
	{

		left = x;
		top = y;
		right = x + PIRANHAPLANT_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}

	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}

#include "MovingEdge.h"
#include "Game.h"



void MovingEdge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MOVING_EDGE_STATE_INACTIVE || isEnable == false)
		return;

	GameObject::Update(dt, coObjects);


	// Khi đụng đến vị trí stop thì sẽ dừng cập nhật
	int screenWidth = Game::GetInstance()->GetScreenWidth();
	if (x >= stopX - SCREEN_WIDTH)
	{
		x = stopX - SCREEN_WIDTH;
		this->SetState(MOVING_EDGE_STATE_INACTIVE);
		return;
	}


	x += dx;

}



void MovingEdge::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MOVING_EDGE_STATE_MOVING:
		vx = MOVING_EDGE_SPEED;
		vy = 0;
		break;
	case MOVING_PLATTFORM_STATE_INACTIVE:
		vx = 0;
		isEnable = false;
		break;
	}
}



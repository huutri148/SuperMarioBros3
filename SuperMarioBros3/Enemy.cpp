#include "Enemy.h"
#include "Game.h"

Enemy::Enemy() :GameObject()
{
	
}

Enemy::~Enemy()
{

}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
}

void Enemy::SetState(int state)
{
	GameObject::SetState(state);
}

void Enemy::SetEntryPosition(int x, int y)
{
	this->entryX = x;
	this->entryY = y;
}

bool Enemy::IsAbleToActive()
{
	if (isEnable = false)
	{
		Game* game = Game::GetInstance();
		int widthX = game->GetCamX() + game->GetScreenWidth();
		int widthY = game->GetCamY() + game->GetScreenHeight();
		if (entryX > widthX || entryX < game->GetCamX())
			return true;
	}
	return false;
}

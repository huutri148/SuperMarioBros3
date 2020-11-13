#include "Enemy.h"
#include "Game.h"

Enemy::Enemy() :GameObject()
{
	deathTime = 0;
	entryX = 0;
	entryY = 0;
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

void Enemy::SetEntryPosition(float x, float y)
{
	this->entryX = x;
	this->entryY = y;
}

bool Enemy::IsAbleToActive()
{
	Game* game = Game::GetInstance();
	float widthX = game->GetCamX() + game->GetScreenWidth();
	float widthY = game->GetCamY() + game->GetScreenHeight();
	if (isEnable == false)
	{
		if (/*entryX >= widthX || entryX < game->GetCamX()*/
			widthX <= entryX - 50 || entryX < game->GetCamX()||
			(game->GetCamX() == 16 && entryX < widthX))
		{
			
			this->isEnable = true;
			this->EnableAgain();
			return true;
		}
		else
			return false;
	}
	return true;
}
void Enemy::EnableAgain()
{
	this->x = entryX;
	this->y = entryY;
}

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

void Enemy::AbleToActive()
{
	Game* game = Game::GetInstance();
	float cam_x = game->GetCamX();
	float cam_y = game->GetCamY();
	if ((this->entryX <= cam_x  || this->entryX > cam_x + game->GetScreenWidth()))
		this->isAbleToActive = true;
}

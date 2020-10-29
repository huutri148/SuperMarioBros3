#include "Enemy.h"

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
	this->entrY = x;
	this->entrY = y;
}


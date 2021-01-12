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
	if ((this->entryX + this->GetWidth() <= cam_x  || this->entryX > cam_x + game->GetScreenWidth()))
		this->isAbleToActive = true;
}


void Enemy::GainScore(int score)
{
	Game* game = Game::GetInstance();
	LPSCENE scene = game->GetCurrentScene();
	int type = 0;
	switch (score)
	{
	case 100:
		type = POINT_TYPE_100;
		break;
	case 200:
		type = POINT_TYPE_200;
		break;
	case 400:
		type = POINT_TYPE_400;
		break;
	case 800:
		type = POINT_TYPE_800;
		break;
	case 1000:
		type = POINT_TYPE_1000;
		break;
	}
	if (dynamic_cast<PlayScene*>(scene))
	{
		Grid* grid = ((PlayScene*)game->GetCurrentScene())->GetGrid();
		PointEffect* effect = new PointEffect(x, y, type);
		Unit* unit = new Unit(grid, effect, x, y);
	}
}
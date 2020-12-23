#include "WorldMapPlayer.h"
#include"Utils.h"
#include"WorldMap.h"
#include"Player.h"
void WorldMapPlayer::Render()
{
	int ani = -1;
	if (form == MARIO_BIG_FORM)
		ani = WORLD_MAP_PLAYER_ANI_BIG;
	else if (form == MARIO_RACCOON_FORM)
		ani = WORLD_MAP_PLAYER_ANI_RACCOON;
	else
		ani = WORLD_MAP_PLAYER_ANI_SMALL;
	animation_set->at(ani)->Render(-1, x, y);
}
void WorldMapPlayer::Render(float translateX, float translateY)
{
	int ani = -1;
	if (form == MARIO_BIG_FORM)
		ani = WORLD_MAP_PLAYER_ANI_BIG;
	else if (form == MARIO_RACCOON_FORM)
		ani = WORLD_MAP_PLAYER_ANI_RACCOON;
	else
		ani = WORLD_MAP_PLAYER_ANI_SMALL;
	animation_set->at(ani)->Render(-1, round(x),round( y), 255, translateX, translateY);
}
void WorldMapPlayer::Up()
{
	if (vx == 0 && vy == 0 && currentPanel->possibleTop == 1)
	{
		vy = -OBJECT_TYPE_PLAYER_SPEED;
	}
		
}
void WorldMapPlayer::Down()
{
	if (vx == 0 && vy == 0 && currentPanel->possibleBottom == 1)
		vy = OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Left()
{
	if (vx == 0 && vy == 0 && currentPanel->possibleLeft == 1)
		vx = -OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Right()
{
	if (vx == 0 && vy == 0 && currentPanel->possibleRight == 1)
		vx = OBJECT_TYPE_PLAYER_SPEED;
}
void WorldMapPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	FindNearestPanel();

	if (nearestPanel->x == currentPanel->x && nearestPanel->y == currentPanel->y)
	{
		vx = 0;
		vy = 0;
		return;
	}
	
	if (vx > 0 && vy == 0)
	{
		if (x < nearestPanel->x)
			x += dx;
		if (x >= nearestPanel->x)
		{
			x = nearestPanel->x;
			vx = 0;
			currentPanel = nearestPanel;
		}
	}
	if (vx < 0 && vy == 0)
	{
		if (x > nearestPanel->x)
			x += dx;
		if (x <= nearestPanel->x)
		{
			x = nearestPanel->x;
			vx = 0;
			currentPanel = nearestPanel;
		}
	}
	if (vy < 0 && vx == 0)
	{
		if (y > nearestPanel->y)
			y += dy;
		if (y <= nearestPanel->y)
		{
			y = nearestPanel->y;
			vy = 0;
			currentPanel = nearestPanel;
		}
	}

	if (vy > 0 && vx == 0)
	{
		if (y < nearestPanel->y)
			y += dy;
		if (y >= nearestPanel->y)
		{
			y = nearestPanel->y;
			vy = 0;
			currentPanel = nearestPanel;
		}
	}
}
void WorldMapPlayer::FindNearestPanel()
{
	if (dynamic_cast<WorldMap*>(Game::GetInstance()->GetCurrentScene()))
	{
		WorldMap* worldMap = (WorldMap*)Game::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT> panels = worldMap->GetPanels();
		if (currentPanel == NULL)
			currentPanel =(WorldMapPanel*) panels.at(0);
		float nearestPanelX = currentPanel->x;
		float nearestPanelY = currentPanel->y;
		if (vx != 0 && vy == 0)
		{
			if (vx > 0)
				nearestPanelX = currentPanel->x + 32;
			if (vx < 0)
				nearestPanelX = currentPanel->x - 32;
			nearestPanelY = currentPanel->y;
		}
		else if (vy != 0 && vx == 0)
		{
			if (vy < 0)
				nearestPanelY = currentPanel->y - 32;
			if (vy > 0)
				nearestPanelY = currentPanel->y + 32;
			nearestPanelX = currentPanel->x;
		}
		for (auto obj : panels)
		{
			if (obj->x == nearestPanelX && obj->y == nearestPanelY)
			{
				nearestPanel = (WorldMapPanel*)obj;
				return;
			}
		}
	}
}
WorldMapPlayer::WorldMapPlayer()
{
	form = Player::GetInstance()->GetLevel();
}

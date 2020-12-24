#include "WorldMapPanel.h"
#include"Player.h"
#include"WorldMap.h"
void WorldMapPanel::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}

void WorldMapPanel::Render(float translateX, float translateY)
{
	if (type == WORLDMAP_PANEL_TYPE_ACTIONSCENE )
	{
		int ani = WORLDMAP_PANEL_ANI_NORMAL;
		if (state == WORLDMAP_PANEL_ANI_CLEARED && sceneId != 0)
			ani = WORLDMAP_PANEL_ANI_CLEARED;
		animation_set->at(ani)->Render(-1, x, y, 255, translateX, translateY);
	}
}
void WorldMapPanel::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case WORLDMAP_PANEL_STATE_CLEARED:
		break;
	}
}
void WorldMapPanel::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (changeStateTime != 0)
	{
		DWORD remain = GetTickCount() - changeStateTime;
		if (remain > WORLDMAP_PANEL_CHANGE_STATE_TIME &&
			remain < WORLDMAP_PANEL_ACTIVE_PLAYER_TIME)
			this->SetState(WORLDMAP_PANEL_STATE_CLEARED);


		// Sau khi changeState sang clear sẽ active lại Player
		else if (remain > WORLDMAP_PANEL_ACTIVE_PLAYER_TIME)
		{
			WorldMap* worldMap = (WorldMap*)Game::GetInstance()->GetCurrentScene();
			worldMap->isChangeState = false;
			changeStateTime = 0;
		}
	}
}
WorldMapPanel::WorldMapPanel(int left, int top, int right, int bottom,int sceneId, int type)
{
	this->sceneId = sceneId;
	this->type = type;
	possibleTop = top;
	possibleLeft = left;
	possibleRight = right;
	possibleBottom = bottom;

	this->SetState(WORLDMAP_PANEL_STATE_NORMAL);



	if (type == WORLDMAP_PANEL_TYPE_ACTIONSCENE)
	{
		Player* player = Player::GetInstance();
		for (unsigned int i = 0; i < player->clearedPanelID.size() - 1; i++)
		{
			if (sceneId == player->clearedPanelID[i])
			{
				this->SetState(WORLDMAP_PANEL_STATE_CLEARED);
				return;
			}
		}
		if (sceneId == player->clearedPanelID[player->clearedPanelID.size() - 1]
			&& player->currentPanelID == sceneId)
		{
			WorldMap* worldMap = (WorldMap*)Game::GetInstance()->GetCurrentScene();
			worldMap->isChangeState = true;
			changeStateTime = GetTickCount();
		}

	}

}
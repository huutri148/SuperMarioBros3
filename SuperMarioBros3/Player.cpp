#include "Player.h"
#include <cstddef>

Player* Player::instance = NULL;
Player* Player::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Player();
		instance->initializePlayer();
	}
	return instance;
}

void Player::initializePlayer()
{
	score = 0;
	money = 0;
	life = 4;
	playerType = OBJECT_TYPE_MARIO;
	form = MARIO_SMALL_FORM;
	currentPanelID = 0;
	clearedPanelID.push_back(currentPanelID);
}

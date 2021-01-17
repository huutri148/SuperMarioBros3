#include "Hud.h"
#include"Game.h"
#include"Player.h"

Hud::Hud()
{
	Textures* textures = Textures::GetInstance();
	Sprites* sprites = Sprites::GetInstance();
	Game* game = Game::GetInstance();
	Player* inPlayer = Player::GetInstance();
	AnimationSets * animation_sets = AnimationSets::GetInstance();

	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATION_SET_HUD_ID);
	this->SetAnimationSet(ani_set);


	powerMelterStack = 0;
	world = 1;
	money = inPlayer->GetMoney();
	score = inPlayer->GetScore();
	nlife = inPlayer->GetLife();
	remainTime = DEFAULT_TIME;

	//Todo: viết hàm xử lí khi Player thu thập đủ 3 Card
	indexTakenCard = inPlayer->card.size();
	if (indexTakenCard >= 3)
	{
		indexTakenCard = 0;
		inPlayer->card.clear();

	}
	for (int i : inPlayer->card)
	{
		cardSprite.push_back(sprites->Get(i));
	}


	if (dynamic_cast<PlayScene*>(game->GetCurrentScene()))
	{
		PlayScene* scene =(PlayScene*)game->GetCurrentScene();
		mario = scene->GetPlayer();
	}

	font = new Font();
	hud1 = sprites->Get(SPRITE_HUD1_ID);
	hud2 = sprites->Get(SPRITE_HUD2_ID);
	
	lifeSprite = font->mapping(nlife +'0');
	worldSprite = font->mapping(world + '0');


	// Sẽ cập nhật player từ scene hiện tại 
	// Là Lugi hoặc Mario sẽ chọn icon ở cuối góc trái màn hình
	int playerType = inPlayer->GetPlayerType();


	if(playerType == OBJECT_TYPE_LUGI)
		playerSprite = sprites->Get(SPRITE_LUGI_PLAYER_ID);
	else
		playerSprite = sprites->Get(SPRITE_MARIO_PLAYER_ID);

	for (unsigned int i = 0; i < POWER_MELTER_FULL; i++)
	{
		if (i != POWER_MELTER_FULL - 1)
			powerMelterSprite.push_back(sprites->Get(SPRITE_POWERMELTER_ID));
		else
			powerMelterSprite.push_back(sprites->Get(SPRITE_POWEMELTER_ARROW_ID));
	}

	for (unsigned int i = 0; i < POWER_MELTER_FULL; i++)
	{
		if (i != POWER_MELTER_FULL - 1)
			filledPowerMelterSprite.push_back(sprites->
				Get(SPRITE_FILLED_POWERMELTER_ID));
		else
			filledPowerMelterSprite.push_back(sprites->
				Get(SPRITE_FILLED_POWERMETER_ARROW_ID));
	}

}
void Hud::Render()
{
	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);
	Game* game = Game::GetInstance();


	//BACKGROUND màu đen
	Game::GetInstance()->Draw( 1 ,  x , y , bbox, 0, 0, game->GetScreenWidth() + 4,
								50, 255,0,-84);



	// Draw(-1,x,y,transparent, translateX( position in X-axis HUD),
	//translateY( position in Y-axis HUD))
	hud1->Draw(-1, x, y, 255, 28, -80);
	hud2->Draw(-1, x ,y, 255, 195, -80);
	playerSprite->Draw(-1, x , y, 255, 32, -65);
	lifeSprite->Draw(-1, x , y, 255, 57, -65);
	worldSprite->Draw(-1, x , y , 255, 65, -73);
	


	for (unsigned int i = 0; i < remainTimeSprites.size(); i++)
	{
		// Draw(-1,x,y,transparent, translateX( position in X-axis HUD),
		//translateY( position in Y-axis HUD)
		remainTimeSprites[i]->Draw(-1, x, y, 255,
					152.0f + FONT_BBOX_WIDTH*i,-65.0f);
	}
	for (unsigned int i = 0; i < scoreSprite.size(); i++)
	{
		scoreSprite[i]->Draw(-1, x , y, 255, 
					80.0f + FONT_BBOX_WIDTH * i, -65.0f);
	}

	for (unsigned int i = 0; i < moneySprite.size(); i++)
	{
		moneySprite[i]->Draw(-1, x , y, 255, 
					160.0f + FONT_BBOX_WIDTH * i, -73.0f);
	}
	
	for (unsigned int i = 0; i < powerMelterSprite.size(); i++)
	{
		powerMelterSprite[i]->Draw(-1, x , y ,255,
					80.0f + FONT_BBOX_WIDTH * i,-73.0f);
	}



	for ( int i = 0; i < powerMelterStack ; i++)
	{
		filledPowerMelterSprite[i]->Draw(-1, x, y,255,
					80.0f + FONT_BBOX_WIDTH * i,-73.0f);
		if (i == 6)
		{
			animation_set->at(0)->Render(-1, x, y, 255,
					80.0f + FONT_BBOX_WIDTH * 6, -73.0f);
		}
	}


	for (unsigned int i = 0; i < cardSprite.size(); i++)
	{
		this->cardSprite[i]->Draw(-1, x, y, 255,
					195 + 24.0f* i, -78.0f);
	}



	if (isGameDone)
	{
		animationTakenCard->Render(-1, x, y, 255, 195 + 24.0f * indexTakenCard, -78.0f);
	}
}
void Hud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Game* game = Game::GetInstance();
	Scene* scene = game->GetCurrentScene();
	Player* player = player->GetInstance();
	Sprites* sprites = Sprites::GetInstance();

	x = game->GetCamX();
	y = game->GetCamY() + SCREEN_HEIGHT;

	// Cộng điểm sau khi done Game
	if (isGameDone)
	{
		if (doneScenePoint != 0)
		{
			player->GainPoint(100);
			doneScenePoint -= 100;
		}
	}


	if (mario != NULL)
	{
		powerMelterStack = mario->GetPowerMelter();
		money = player->GetMoney();
		score = player->GetScore();
		nlife = player->GetLife();
		time += dt;
		remainTime = DEFAULT_TIME - time / 1000;
	}


	lifeSprite = font->mapping(nlife + '0');

	string time_str = to_string(remainTime);
	while (time_str.length() < 3) time_str = "0" + time_str;
	remainTimeSprites = font->StringToSprite(time_str);

	string score_str = to_string(score);
	while (score_str.length() < 7) score_str = "0" + score_str;
	scoreSprite = font->StringToSprite(score_str);

	string money_str = to_string(money);
	moneySprite = font->StringToSprite(money_str);
}
void Hud::DoneGame(int idCard)
{
	isGameDone = true;
	doneScenePoint = DONE_SCENE_POINT;
	idTakenCard = idCard;
	
	// Todo: Có thể dùng Factory Pattern cho Card ???
	if(idCard == SPRITE_CARD_MUSHROOM)
		animationTakenCard = Animations::GetInstance()->Get(ANIMATION_TAKEN_MUSHROOM_CARD);
	else if(idCard == SPRITE_CARD_STARMAN)
		animationTakenCard = Animations::GetInstance()->Get(ANIMATION_TAKEN_STAR_CARD);
	else 
		animationTakenCard = Animations::GetInstance()->Get(ANIMATION_TAKEN_FIREFLOWER_CARD);
}

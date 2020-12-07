#include "Hud.h"
#include"Game.h"
// Todo: Fix Hard code
// and fix how power melter work
// need to scale HUD to fit with screen 
Hud::Hud()
{
	// Shoud I make it an instance so i won't have to 
	// access all of these thing ????
	Textures* textures = Textures::GetInstance();
	Sprites* sprites = Sprites::GetInstance();
	Game* game = Game::GetInstance();
	Scene* scene = game->GetCurrentScene();
	if(dynamic_cast<PlayScene*>(scene))
	{
		Mario* mario = ((PlayScene*)game->GetCurrentScene())->GetPlayer();
		powerMelterStack = mario->GetPowerMelter();
		money = mario->GetMoney();
		score = mario->GetScore();
		nlife = mario->GetLife();
	}
	
	font = new Font();
	hud1 = sprites->Get(50001);
	hud2 = sprites->Get(50002);
	lifeSprite = font->mapping(nlife +'0');
	worldSprite = font->mapping(world + '0');
	// Sẽ cập nhật player từ scene hiện tại 
	// Là Lugi hoặc Mario sẽ chọn icon ở cuối góc trái màn hình
	playerSprite = sprites->Get(50003);
	for (unsigned int i = 0; i < 7; i++)
	{
		if (i != 6)
			powerMelterSprite.push_back(sprites->Get(50007));
		else
			powerMelterSprite.push_back(sprites->Get(50008));
	}
	for (unsigned int i = 0; i < 7; i++)
	{
		if (i != 6)
			filledPowerMelterSprite.push_back(sprites->Get(50005));
		else
			filledPowerMelterSprite.push_back(sprites->Get(50006));
	}
}
void Hud::Render()
{
	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);
	Game* game = Game::GetInstance();
	Game::GetInstance()->Draw(1,x , y , bbox, 0, 0, game->GetScreenWidth() , 40, 255,0,-84);
	hud1->Draw(-1, x, y, 255, 28, -80);
	hud2->Draw(-1, x ,y, 255, 195, -80);
	playerSprite->Draw(-1, x , y, 255, 32, -65);
	lifeSprite->Draw(-1, x , y, 255, 57, -65);
	worldSprite->Draw(-1, x , y , 255, 65, -73);
	
	for (unsigned int i = 0; i < remainTimeSprites.size(); i++)
	{
		remainTimeSprites[i]->Draw(-1, x, y, 255, 152.0f + FONT_BBOX_WIDTH*i,-65.0f);
	}
	for (unsigned int i = 0; i < scoreSprite.size(); i++)
	{
		scoreSprite[i]->Draw(-1, x , y, 255, 80.0f + FONT_BBOX_WIDTH * i, -65.0f);
	}

	for (unsigned int i = 0; i < moneySprite.size(); i++)
	{
		moneySprite[i]->Draw(-1, x , y, 255, 160.0f + FONT_BBOX_WIDTH * i, -73.0f);
	}
	
	for (unsigned int i = 0; i < powerMelterSprite.size(); i++)
	{
		powerMelterSprite[i]->Draw(-1, x , y ,255, 80.0f + FONT_BBOX_WIDTH * i,-73.0f);
	}
	// Todo: needed a way to deal with it more efficiently
	// and it work differently in raccoon form
	for ( int i = 0; i < powerMelterStack ; i++)
	{
		filledPowerMelterSprite[i]->Draw(-1, x, y,255, 80.0f + FONT_BBOX_WIDTH * i,-73.0f);
	}
}
void Hud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Game* game = Game::GetInstance();
	Scene* scene = game->GetCurrentScene();
	x = game->GetCamX();
	y = game->GetCamY() + SCREEN_HEIGHT;
	if (dynamic_cast<PlayScene*>(scene))
	{
		Mario* mario = ((PlayScene*)game->GetCurrentScene())->GetPlayer();
		powerMelterStack = mario->GetPowerMelter();
		money = mario->GetMoney();
		score = mario->GetScore();
		nlife = mario->GetLife();
		time += dt;
		remainTime = 300 - time / 1000;
	

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
}

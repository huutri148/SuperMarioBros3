#include "Hud.h"
#include"Game.h"
// Todo: Fix Hard code
// and fix how power melter work
// need to scale HUD to fit with screen 
Hud::Hud()
{
	// Shoud I make it an instance so i won't have to 
	// access all of this thing ????
	Textures* textures = Textures::GetInstance();
	Sprites* sprites = Sprites::GetInstance();
	Game* game = Game::GetInstance();
	Mario* mario = ((PlayScene*)game->GetCurrentScene())->GetPlayer();
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

	powerMelterStack = mario->GetPowerMelter();
	money = mario->GetMoney();
	score = mario->GetScore();
	nlife = mario->GetLife();
	

}
void Hud::Render()
{
	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);
	Game* game = Game::GetInstance();
	Game::GetInstance()->Draw(x + 5, y - 84
		, bbox, 0, 0, game->GetScreenWidth() - 5, 40, 255);
	hud1->Draw(-1, x + 40, y - 78);
	hud2->Draw(-1, x + 220, y  - 78);
	playerSprite->Draw(-1, x + 44, y - 63);
	lifeSprite->Draw(-1, x + 69, y  - 63);
	worldSprite->Draw(-1, x + 77, y - 71);
	
	for (unsigned int i = 0; i < remainTimeSprites.size(); i++)
	{
		remainTimeSprites[i]->Draw(-1, x + 164 + 8 * i, y - 63);
	}
	for (unsigned int i = 0; i < scoreSprite.size(); i++)
	{
		scoreSprite[i]->Draw(-1, x + 92 + 8 * i, y  - 63);
	}

	for (unsigned int i = 0; i < moneySprite.size(); i++)
	{
		moneySprite[i]->Draw(-1, x + 172 + 8 * i, y - 71);
	}
	
	for (unsigned int i = 0; i < powerMelterSprite.size(); i++)
	{
		powerMelterSprite[i]->Draw(-1, x + 92 + 8 * i, y - 71);
	}
	// Todo: needed a way to deal with it more efficiently
	// and it work differently in raccoon form
	for ( int i = 0; i < powerMelterStack ; i++)
	{
		filledPowerMelterSprite[i]->Draw(-1, x + 92 + 8 * i, y - 71);
	}
}
void Hud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Game* game = Game::GetInstance();
	Mario* mario = ((PlayScene*)game->GetCurrentScene())->GetPlayer();
	x = game->GetCamX();
	y = game->GetCamY() + SCREEN_HEIGHT;

	money = mario->GetMoney();
	score = mario->GetScore();
	nlife = mario->GetLife();
	powerMelterStack = mario->GetPowerMelter();

	time += dt;
	remainTime = 300 - time/1000;
	
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

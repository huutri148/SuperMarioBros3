#include "BackGroundIntroScene.h"
#include"Game.h"
BackGroundIntroScene::BackGroundIntroScene()
{
	Sprites* sprites = Sprites::GetInstance();
	scrollingCurtain = sprites->Get(SPRITE_SCROLLING_CURTAIN_ID);
	ground = sprites->Get(SPRITE_GROUND_ID);
	for (int i = 0; i < 2; i++)
	{
		backGrounds.push_back(sprites->Get(SPRITE_BACKGROUND_ID + i));
	}
	vy = -SCROLLING_SPEED;
}
void BackGroundIntroScene::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Game* game = Game::GetInstance();
	float cam_y = game->GetCamY();
	// Khi màn cuộn được một nửa sẽ bắt đầu Section1
	if (y <  cam_y - SCROLLING_CURTAIN_HEIGHT/2 
		&& isDoneSection1 == false)
	{
		isBeginSection1 = true;
	}
	// Done Scenetion1
	if (isBeginSection1 == true)
	{
		if (y < cam_y - SCROLLING_CURTAIN_HEIGHT )
		{
			isBeginSection1 = false;
			isDoneSection1 = true;
			isBeginSection2 = true;
			beginSection2Time = GetTickCount();
		}
	}
	if (isBeginSection3 == true )
	{
		if (section3Time == 0)
		{
			if (shakingTime == 0)
			{
				indexBackGround = 0;
				vy = BACKGROUND_SHAKING_SPEED;
				if (y > 10)
				{
					y = 10;
					shakingTime = GetTickCount();
				}
			}
			else if (GetTickCount() - shakingTime < SECTION3_SHAKING_TIME)
			{
				if (vy < 0)
					vy = BACKGROUND_SHAKING_SPEED;
				else
					vy = -BACKGROUND_SHAKING_SPEED;
			}
			else
			{
				section3Time = GetTickCount();
				vy = 0;
			}

		}
		else if (GetTickCount() - section3Time > SECTION3_CHANGE_BACKGROUND_TIME)
		{
			indexBackGround = 1;
			y = 13;
			//isBeginSection3 = false;
			isDoneSection3 = true;
		}
	}
	GameObject::Update(dt);
	y += dy;
}

void BackGroundIntroScene::Render()
{
	//animation_set->at(0)->Render(-1, x, y);
	if (isDoneSection1 == false)
	{
		scrollingCurtain->Draw(-1, round(16.0f), round(y));
	}
	if (isBeginSection3 == true)
	{
		backGrounds.at(indexBackGround)->Draw(-1, round(16.0f), round(y));
		if(indexBackGround == 1)
			animation_set->at(0)->Render(-1, x + BACKGROUND_BBOX_WIDTH/2, y + BACKGROUND_BBOX_HEIGHT/2 +3, 255);
	}
	ground->Draw(-1 ,16,SCREEN_HEIGHT - 89,255,0,0);
}

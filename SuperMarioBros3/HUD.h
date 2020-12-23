#pragma once
#include"GameObject.h"
#include"Sprites.h"
#include"Font.h"
#include"Mario.h"
class Hud : public GameObject
{
	Font* font;
	Mario* mario = NULL;



	LPSPRITE hud1;
	LPSPRITE hud2;
	vector<LPSPRITE> cardSprite;
	LPSPRITE playerSprite;
	LPSPRITE lifeSprite;
	vector<LPSPRITE> moneySprite;
	LPSPRITE worldSprite;
	vector<LPSPRITE> scoreSprite;
	vector<LPSPRITE> remainTimeSprites;
	vector<LPSPRITE> powerMelterSprite;
	vector<LPSPRITE> filledPowerMelterSprite;
	
	int nlife = 4;
	int remainTime = 0;
	int score = 0;
	int money = 0;
	int world = 1;
	int time = 0;
	int powerMelterStack = 0;

	int doneScenePoint;
	int indexTakenCard;
	int idTakenCard;
	int isGameDone = false;
	vector<int> card;
public:
	Hud();
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void DoneGame(int idCard);
};


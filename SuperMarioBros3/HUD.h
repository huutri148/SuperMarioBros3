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
	LPSPRITE playerSprite;
	LPSPRITE lifeSprite;
	LPSPRITE worldSprite;

	vector<LPSPRITE> moneySprite;
	vector<LPSPRITE> cardSprite;
	vector<LPSPRITE> scoreSprite;
	vector<LPSPRITE> remainTimeSprites;
	vector<LPSPRITE> powerMelterSprite;
	vector<LPSPRITE> filledPowerMelterSprite;
	Animation* animationTakenCard;


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
	~Hud();
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual int GetWidth() { return 0; };
	void DoneGame(int idCard);
};


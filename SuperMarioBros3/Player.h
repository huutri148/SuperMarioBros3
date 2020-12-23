#pragma once
#include"Define.h"
#include<vector>
using std::vector;
class Player
{
	static Player* instance;
	int playerType;
	int score = 0;
	int money = 0;
	int life = 4;
	int form;
	
public: 
	vector<int> card;
	static Player* GetInstance();
	void initializePlayer();

	int GetLevel() { return form; };
	int GetMoney() { return money; };
	int GetScore() { return score; };
	int GetLife() { return life; };
	int GetPlayerType() { return playerType; };

	void SetLevel(int form) { this->form = form; };
	void TakeCard(int idCard) { this->card.push_back(idCard); };
	
	void GainMoney(int pMoney) { money += pMoney; };
	void GainPoint(int pPoint) { score += pPoint; };
	void GainLife() { life += 1; };
	void LoseLife() { life -= 1; };

};


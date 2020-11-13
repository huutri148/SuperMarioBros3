#pragma once
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "Scene.h"
#include "Map.h"
#include"Block.h"
#include"Ground.h"
#include "Pipe.h"
#include"FireBall.h"
#include"PiranhaPlant.h"
#include"Coin.h"
#include"FirePiranhaPlant.h"
#include"FirePlantBullet.h"
#include"RaccoonLeaf.h"
#include"Mushroom.h"
#include"Item.h"

class PlayScene : public Scene
{
protected:
	Mario* player = NULL;
	vector<LPGAMEOBJECT> objects ;
	Map* map = NULL ;
	bool _turnCamY = false;
	FirePlantBullet* firebullet = NULL;
	/*Item* item;*/
	RaccoonLeaf* leaf = NULL;
	Mushroom* mushroom = NULL;
	Coin* coin = NULL;
	//LPFireBall fireball;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);

public:
	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void TurnCamY(float _playerY, bool isFlying, int ScreenHeight, int MapHeight);
	Mario* GetPlayer() { return player; }
	FirePlantBullet* GetPlantBullet() { return firebullet; }
	void AddObject(GameObject* obj);
	RaccoonLeaf* GetLeaf() { return leaf; }
	Mushroom* GetMushroom() { return mushroom; }
	Coin* GetCoin() { return coin; };
	//LPFireBall GetFireBall() { return fireball; }
	friend class CPlayScenceKeyHandler;

};

class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	
	PlayScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};


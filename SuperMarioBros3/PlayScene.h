﻿#pragma once
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
#include"PSwitch.h"
#include "Grid.h"

class PlayScene : public Scene
{
protected:
	Mario* player = NULL;
	vector<LPGAMEOBJECT> objects ;
	Map* map = NULL ;
	bool _turnCamY = false;
	FirePlantBullet* firebullet = NULL;
	RaccoonLeaf* leaf = NULL;
	Mushroom* mushroom = NULL;
	Coin* coin = NULL;
	PSwitch* pSwitch = NULL;
	int xn = 0;
	Unit* unit;
	Grid* grid;
	vector<Unit*> listUnits;


	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listStaticObjectsToRender;
	vector<LPGAMEOBJECT> listMovingObjectsToRender;


	//LPFireBall fireball;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);
	void _ParseSection_GRID(string line);
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
	PSwitch* GetSwitch() { return pSwitch; };
	//LPFireBall GetFireBall() { return fireball; }
	void ActiveEnemiesInViewport();



	void SetInactivation();
	bool IsInViewport(LPGAMEOBJECT object);
	void UpdateCameraPosition();
	
	// Nhận các đối tượng cần xét
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects);
	void GetObjectFromGrid();
	void UpdateGrid();
	void UpdatePlayer(DWORD dt);


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


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
#include"PSwitch.h"
#include"ParaGoomba.h"
#include "Grid.h"
#include"KoopaParaTroopa.h"
#include"PointEffect.h"
#include"Hud.h"
#include"HitEffect.h"
#include"Portal.h"
class PlayScene : public Scene
{
protected:
	Portal* portal;
	Mario* player = NULL;
	Map* map = NULL ;
	bool isTurnCamY = false;// Camera Y được bật khi Mario bay 
	bool isGameOver = false;
	Hud* hud = NULL;
	Unit* unit;
	Grid* grid;
	
	vector<LPGAMEOBJECT> objects;
	vector<Unit*> listUnits;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listStaticObjectsToRender;
	vector<LPGAMEOBJECT> listMovingObjectsToRender;

	/*vector<LPGAMEOBJECT> listPipeToRender; */// Pipe phải vẽ đè lên plant


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
	

	// Bật tắt quái khi không nằm trong viewport
	void ActiveEnemiesInViewport();
	void SetInactivation();
	bool IsInViewport(LPGAMEOBJECT object);

	void UpdateCameraPosition();
	
	// Nhận các đối tượng cần xét
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects);
	void GetObjectFromGrid();
	Grid* GetGrid() { return grid; };
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


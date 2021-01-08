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
#include"RaccoonTail.h"
#include"BrokenBrickEffect.h"
#include"RedKoopaParaTroopa.h"
#include"MovingPlattform.h"
#include"BoomerangBrother.h"
#include"MovingEdge.h"



class PlayScene : public Scene
{
protected:


	DWORD delayGameOverTime = 0;


	Portal* portal = NULL;
	Mario* player = NULL;
	Map* map = NULL ;
	Hud* hud = NULL;
	MovingEdge* movingEdge = NULL;
	Unit* unit;
	Grid* grid;


	bool switchScene = false;
	bool isGameDone = false;
	bool isGameOver = false;
	bool isTurnCamY = false;	// Camera Y được bật khi Mario bay 




	vector<LPGAMEOBJECT> objects;
	vector<Unit*> listUnits;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listStaticObjectsToRender;
	vector<LPGAMEOBJECT> listMovingObjectsToRender;
	vector<LPGAMEOBJECT> listPipesToRender;
	vector<LPGAMEOBJECT> listWeaponsToRender;



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


	Mario* GetPlayer() { return player; }
	float GetEdgeLeft();
	float GetEdgeRight();
	float GetEdgeRightExtraMap() { return map->edgeRightInExtraMap; }
	float GetEdgeLeftExtraMap() { return map->edgeLeftInExtraMap; }
	void GetExtraMapPosition(float& x, float& y);
	void GetWorldMapPosition(float& x, float& y);
	void GetStartPosition(float& x, float& y);
	int GetTakenCardID() { return portal->GetCardId(); };
	void ParseObjFromFile(LPCWSTR path);

	// Nhận các đối tượng cần xét va chạm
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects);
	void GetObjectFromGrid();
	Grid* GetGrid() { return grid; };
	MovingEdge* GetMovingEdge() { return movingEdge; };


	void SetDoneGame() { isGameDone = true; };
	void DoneGame();

	

	// Bật tắt quái khi không nằm trong viewport
	void ActiveEnemiesInViewport();
	void SetInactivation();
	bool IsInViewport(LPGAMEOBJECT object);
	void TurnCamY(float _playerY, bool isFlying, int ScreenHeight, int MapHeight);


	void UpdateCameraPosition();
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


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
class PlayScene : public Scene
{
protected:
	Mario* player;				
	vector<LPGAMEOBJECT> objects;
	Map* map;
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

	Mario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlayScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};


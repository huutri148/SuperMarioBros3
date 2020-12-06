#pragma once
#include"Scene.h"
#include"Hud.h"
#include"Map.h"
#include"Utils.h"
#include"Game.h"
class WorldMap : public Scene 
{
protected:
	Hud* hud = NULL;
	Map* tileMap = NULL;
	//vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);

public:
	WorldMap(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	friend class WorldMapKeyHandler;
};

class WorldMapKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	WorldMapKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};

#pragma once
#include"Scene.h"
#include"Hud.h"
#include"Map.h"
#include"Utils.h"
#include"Game.h"
#include"WorldMapBush.h"
#include"WorldMapPanel.h"
#include"WorldMapPlayer.h"
#include"Hud.h"
class WorldMap : public Scene
{
protected:
	Hud* hud = NULL;
	Map* tileMap = NULL;
	WorldMapPlayer* player;
	vector<LPGAMEOBJECT> objectsToRender;
	vector<WorldMapPanel*> panels;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);

public:
	bool isChangeState = false;
	WorldMap(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	
	WorldMapPlayer* GetPlayer() { return player; };
	vector<WorldMapPanel*> GetPanels() { return panels; };
	WorldMapPanel* FindCurrentPanel(int sceneId);
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

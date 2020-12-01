#pragma once
#include"Scene.h"
#include"Game.h"
class IntroScene :public Scene
{
	Mario* player1;
	Mario* player2;





	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line);
	void _ParseSection_GRID(string line);
public:
	IntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	friend class IntroSceneHandler;
};
class IntroSceneKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	IntroSceneKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};

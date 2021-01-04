#pragma once
#include"Scene.h"
#include"Game.h"
#include"Goomba.h"
#include"BackGroundIntroScene.h"
class IntroScene :public Scene
{
	Mario* mario;
	Mario* lugi;
	DWORD changeAniTime;

	unsigned int indexMenu = 0;
	vector<LPSPRITE> menuSprites;
	BackGroundIntroScene* backGround;
	vector<LPSPRITE> bushsSprite;


	Goomba* goomba;
	Enemy* koopa;
	vector<LPGAMEOBJECT> fallingObjects;
	vector<LPGAMEOBJECT> koopas;
	vector<LPGAMEOBJECT> objects;



	bool canJumpToCounter = false;
	bool marioPickShell = false;
	bool showingKoopas = false;
	bool showingBush = false;
	bool showingMenu = false;

	




	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	IntroScene(int id, LPCWSTR filePath);

	void ShowBush();
	void ShowKoopas();
	void ShowMenu();
	void SwitchMenu();


	void HandleBehaviourOfMario();
	void HandleBehaviourOfLugi();


	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();


	Mario* GetMario() { return mario; };
	Mario* GetHoldingPlayer();



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

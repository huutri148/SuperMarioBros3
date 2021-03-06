﻿#include "WorldMap.h"
#include<fstream>
#include"Player.h"
WorldMap::WorldMap(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new WorldMapKeyHandler(this);
}
void WorldMap::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void WorldMap::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}
	Sprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void WorldMap::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	Animations::GetInstance()->Add(ani_id, ani);
}

void WorldMap::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());
	LPANIMATION_SET s = new AnimationSet();
	
	Animations* animations = Animations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	AnimationSets::GetInstance()->Add(ani_set_id, s);
}
void WorldMap::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());


	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_BUSH:
		obj = new WorldMapBush();
		objectsToRender.push_back(obj);
		obj->SetPosition(x, y);
		break;
	case OBJECT_TYPE_PANEL:
	{
		int l = atoi(tokens[4].c_str());
		int t = atoi(tokens[5].c_str());
		int r = atoi(tokens[6].c_str());
		int b = atoi(tokens[7].c_str());
		int sceneId = atoi(tokens[8].c_str());
		int type = atoi(tokens[9].c_str());
		obj = new WorldMapPanel(l, t, r, b, sceneId, type);
		panels.push_back((WorldMapPanel*)obj);
		obj->SetPosition(x, y);
		break;
	}
	case OBJECT_TYPE_PLAYER:
		obj = new WorldMapPlayer();
		player =(WorldMapPlayer*)obj;
		break;
	case OBJECT_TYPE_HUD:
		obj = new Hud();
		hud =(Hud*) obj;
		obj->SetPosition(x, y);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	// General object setup
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
}
void WorldMap::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idMap = atoi(tokens[0].c_str());
	int tolRowTileSet = atoi(tokens[1].c_str());
	int tolColTileSet = atoi(tokens[2].c_str());
	int tolRowMap = atoi(tokens[3].c_str());
	int tolColMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());

	wstring MatrixPath = ToWSTR(tokens[6]);

	float edgeLeft = (float)atof(tokens[7].c_str());
	float edgeRight = (float)atof(tokens[8].c_str());
	float edgeBottomInWorld = (float)atof(tokens[9].c_str());
	float edgeTop = (float)atof(tokens[10].c_str());

	float edgeLeftInExtraMap = (float)atof(tokens[11].c_str());
	float edgeRightInExtraMap = (float)atof(tokens[12].c_str());
	float edgeTopInExtraMap = (float)atof(tokens[13].c_str());
	float edgeBottomInExtraMap = (float)atof(tokens[14].c_str());


	float startPositionX = (float)atof(tokens[15].c_str());
	float startPositionY = (float)atof(tokens[16].c_str());

	float extraMapPositionX = (float)atof(tokens[17].c_str());
	float extraMapPositionY = (float)atof(tokens[18].c_str());

	float worldMapPositionX = (float)atof(tokens[19].c_str());
	float worldMapPositionY = (float)atof(tokens[20].c_str());

	this->tileMap = new Map(idMap, tolRowTileSet, tolColTileSet,
						tolRowMap, tolColMap, totalTiles,
						edgeLeft, edgeRight, edgeBottomInWorld, edgeTop,
						edgeLeftInExtraMap, edgeRightInExtraMap,
						edgeTopInExtraMap, edgeBottomInExtraMap,
						startPositionX, startPositionY,
						extraMapPositionX, extraMapPositionY,
						worldMapPositionX, worldMapPositionY);

	tileMap->LoadMatrix(MatrixPath.c_str());
	tileMap->CreateTilesFromTileSet();
	DebugOut(L"\nParseSection_MAPS: Done");
}
void WorldMap::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines
		if (line == "[MAP]")
		{
			section = SCENE_SECTION_MAPS;
			continue;
		}
		if (line == "[TEXTURES]")
		{
			section = SCENE_SECTION_TEXTURES;
			continue;
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES;
			continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS;
			continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS;
			continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS;
			continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAPS:	_ParseSection_MAPS(line); break;
		}
	}

	f.close();

	Textures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void WorldMap::Update(DWORD dt)
{
	if (isOutOfLife == true)
		return;
	
	player->Update(dt);
	hud->Update(dt);
	if (Player::GetInstance()->GetLife() == 0)
	{
		isOutOfLife = true;
		if (gameOverMenu == NULL)
			gameOverMenu = Sprites::GetInstance()->Get(10098);
		return;
	}
	for (unsigned int i = 0; i < panels.size(); i++)
	{
		panels[i]->Update(dt);
	}
	if (switchScene)
	{
		Game* game = Game::GetInstance();
		int screenWidth = game->GetScreenWidth();
		float translateX = (float)screenWidth / POINT_TO_STOP;
		if (dBackGround < translateX)
			dBackGround += WORLDMAP_BACKGROUND_SPEED * dt;
		else
			ChooseLevel();
	}
}

void WorldMap::Render()
{
	Game* game = Game::GetInstance();
	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	float camX = game->GetCamX();
	float camY = game->GetCamY();


	float translateX = (float)screenWidth / 2 - 130;
	float translateY = (float)screenHeight / 2 - 100;
	

	this->tileMap->Render(camX, camY, screenWidth, screenHeight,
		translateX,translateY);

	

	for (auto obj : objectsToRender)
	{
		obj->Render(round(translateX), round(translateY));
	}

	for (auto panel : panels)
	{
		panel->Render(round(translateX), round(translateY));
	}

	if(!isChangeState)
		player->Render(round(translateX ), round(translateY ));
	if (isOutOfLife)
		gameOverMenu->Draw(-1, round(translateX), round(translateY), 255,tileMap->GetMapWidth()/4.0f,
			tileMap->GetMapHeiht()/4.0f);
	if (switchScene)
	{
		LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);
		//BACKGROUND màu đen
		game->Draw(-1,-(float)game->GetScreenWidth(),0, bbox, 0, 0,SCREEN_WIDTH,
			SCREEN_HEIGHT, 255, dBackGround , 0);
		game->Draw(-1, 0, -(float)game->GetScreenHeight(), bbox, 0, 0, SCREEN_WIDTH,
			SCREEN_HEIGHT, 255, 0, dBackGround);
		game->Draw(-1, 0, (float)game->GetScreenHeight(), bbox, 0, 0, SCREEN_WIDTH,
			SCREEN_HEIGHT, 255, 0, -dBackGround);
		game->Draw(-1 , (float)game->GetScreenWidth(),0, bbox, 0, 0, SCREEN_WIDTH,
			SCREEN_HEIGHT, 255, -dBackGround, 0);
	}
	hud->Render();
}

WorldMapPanel* WorldMap::FindCurrentPanel(int sceneID)
{
	for (WorldMapPanel* panel : panels)
	{
		if (sceneID == panel->GetSceneId() && panel->type == 1)
			return panel;
	}
	return NULL;
}

void WorldMap::Unload()
{
	for (unsigned int i = 0; i < objectsToRender.size(); i++)
		delete objectsToRender[i];
	for (unsigned int i = 0; i < panels.size(); i++)
		delete panels[i];

	delete tileMap;
	delete player;
	delete hud;

	objectsToRender.clear();
	panels.clear();
	tileMap = NULL;
	hud = NULL;
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void WorldMap::ChooseLevel()
{
	if (player->currentPanel->GetSceneId() == 1)
	{
		Game* game = Game::GetInstance();
		Player::GetInstance()->currentPanelID = player->currentPanel->GetSceneId();
		game->SwitchScene(1);
		switchScene = false;
	}
	else if (player->currentPanel->GetSceneId() == 4)
	{
		Game* game = Game::GetInstance();
		Player::GetInstance()->currentPanelID = player->currentPanel->GetSceneId();
		game->SwitchScene(4);
		switchScene = false;
	}
}
void WorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (((WorldMap*)scence)->isChangeState)
		return;
	WorldMapPlayer* player = ((WorldMap*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_LEFT:
		if(!(Player::GetInstance()->GetLife() == 0))
			player->Left();
		break;
	case DIK_DOWN:
		if (!(Player::GetInstance()->GetLife() == 0))
			player->Down();
		break;
	case DIK_RIGHT:
		if (!(Player::GetInstance()->GetLife() == 0))
			player->Right();
		break;
	case DIK_UP:
		if (!(Player::GetInstance()->GetLife() == 0))
			player->Up();
		break;
	case DIK_S:
		if (!(Player::GetInstance()->GetLife() == 0))
		{
			if ((player->currentPanel->GetSceneId() == 1) || (player->currentPanel->GetSceneId() == 4))
				((WorldMap*)scence)->switchScene = true;
		}
		break;
	case DIK_A:
		if ((Player::GetInstance()->GetLife() == 0))
		{
			Player::GetInstance()->Reset();
			Player::GetInstance()->GainLife();
			((WorldMap*)scence)->isOutOfLife = false;
		}
	}
}
void WorldMapKeyHandler::OnKeyUp(int KeyCode)
{

}
void WorldMapKeyHandler::KeyState(BYTE* states)
{
}


#include "WorldMap.h"
#include<fstream>
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
		panels.push_back(obj);
		break;
	}
	case OBJECT_TYPE_PLAYER:
		obj = new WorldMapPlayer();
		player =(WorldMapPlayer*)obj;
		break;
	case OBJECT_TYPE_HUD:
		obj = new Hud();
		hud =(Hud*) obj;
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	// General object setup
	obj->SetPosition(x, y);
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

	this->tileMap = new Map(idMap, tolRowTileSet, tolColTileSet, tolRowMap, tolColMap, totalTiles);
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
	player->Update(dt);
	hud->Update(dt);
}

void WorldMap::Render()
{
	Game* game = Game::GetInstance();
	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	float translateX = (float)screenWidth / 2 - 130;
	float translateY = (float)screenHeight / 2 - 100;
	float cam_x = game->GetCamX();
	float cam_y = game->GetCamY();
	this->tileMap->Render(cam_x, cam_y, screenWidth, screenHeight,
		translateX,translateY);
	for (auto obj : objectsToRender)
	{
		obj->Render(round(translateX), round(translateY));
	}
	for (auto panel : panels)
	{
		panel->Render(round(translateX), round(translateY));
	}
	player->Render(round(translateX ), round(translateY ));
	hud->Render();
}


void WorldMap::Unload()
{
	//for (unsigned int i = 0; i < objectsToRender.size(); i++)
	//	delete objectsToRender[i];
	//for (unsigned int i = 0; i < objectsToRender.size(); i++)
	//	delete panels[i];
	////objectsToRender.clear();
	//panels.clear();
	////tileMap = NULL;
	////hud = NULL;
	//player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void WorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	WorldMapPlayer* player = ((WorldMap*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_A:
		player->Left();
		break;
	case DIK_S:
		player->Down();
		break;
	case DIK_D:
		player->Right();
		break;
	case DIK_W:
		player->Up();
		break;
	case DIK_J:
		break;
	case DIK_K:
	{
		if (player->currentPanel->GetSceneId() == 1)
		{
			Game* game = Game::GetInstance();
			game->SwitchScene(2);
		}
		break;
	}
	}
}
void WorldMapKeyHandler::OnKeyUp(int KeyCode)
{

}
void WorldMapKeyHandler::KeyState(BYTE* states)
{
}


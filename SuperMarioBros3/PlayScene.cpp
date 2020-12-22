﻿#include "PlayScene.h"
#include<iostream>
#include<fstream>
#include "Define.h"
#include"Utils.h"
#include"Textures.h"
#include"Sprites.h"


 
using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new PlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/


void PlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines

	int W = atoi(tokens[0].c_str());
	int H = atoi(tokens[1].c_str());
	int x = atoi(tokens[2].c_str());
	int y = atoi(tokens[3].c_str());

	grid = new Grid(W, H, x, y);
	DebugOut(L"\nParseSection_GRID: Done");
}
void PlayScene::_ParseSection_TEXTURES(string line)
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

void PlayScene::_ParseSection_SPRITES(string line)
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

void PlayScene::_ParseSection_ANIMATIONS(string line)
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

void PlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) 
		return;
	int ani_set_id = atoi(tokens[0].c_str());

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

/*
	Parse a line in section [OBJECTS]
*/
void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());




	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new Mario();
		player = (Mario*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: 
	{
		obj = new Goomba(x, y);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_BRICK:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new Brick(x, y, set_type);
		unit = new Unit(grid, obj, x, y);
		int ani_set_id = atoi(tokens[3].c_str());
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		break;
	}
	case OBJECT_TYPE_KOOPAS:
	{
		int set_type = atoi(tokens[3].c_str());
		obj = new KoopaTroopa(x, y, set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_BLOCKS: 
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		obj = new Block(x, y, width, height);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_GROUNDS:
	{
		obj = new Ground();
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_PIPES: 
	{
		int ani_set_id = atoi(tokens[3].c_str());
		int set_type = atoi(tokens[4].c_str());
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj = new Pipe(set_type);
		obj->SetAnimationSet(ani_set);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT:
	{
		int set_type = atoi(tokens[3].c_str());
		obj = new PiranhaPlant(x, y, set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_FIREPIRANHAPLANT:
	{
		float limit =(float) atof(tokens[3].c_str());
		int set_type = atoi(tokens[4].c_str());
		obj = new FirePiranhaPlant(x, y,limit, set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		obj = new Portal();
		portal =(Portal*)obj;
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_COIN:
	{
		int set_type = atoi(tokens[3].c_str());
		obj = new Coin(set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_PARAGOOMBA:
	{
		obj = new ParaGoomba(x,y);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_PARATROOPA:
	{
		obj = new KoopaParaTroopa(x, y);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_HUD:
	{
		hud = new Hud();
		return;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	// General object setup
	obj->SetPosition(x, y);

}

void PlayScene::_ParseSection_MAPS(string line)
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

	this->map = new Map(idMap, tolRowTileSet, tolColTileSet,
		tolRowMap, tolColMap, totalTiles,
		edgeLeft, edgeRight, edgeTop, edgeBottomInWorld,
		edgeLeftInExtraMap, edgeRightInExtraMap,
		edgeTopInExtraMap, edgeBottomInExtraMap,
		startPositionX, startPositionY,
		extraMapPositionX, extraMapPositionY,
		worldMapPositionX, worldMapPositionY);


	map->LoadMatrix(MatrixPath.c_str());
	map->CreateTilesFromTileSet();
	DebugOut(L"\nParseSection_MAPS: Done");
}

void PlayScene::Load()
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
		if (line == "[GRID]")
		{
			section = SCENE_SECTION_GRID;
			continue;
		}
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
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
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
			
		}
	}

	f.close();

	Textures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void PlayScene::Update(DWORD dt)
{

	GetObjectFromGrid();
	UpdatePlayer(dt);
	ActiveEnemiesInViewport();


	if (portal->state == PORTAL_STATE_CONGRATULATE)
	{
		this->isGameDone = true;
		Game* game = Game::GetInstance();
		game->SetCamPos(0.0f, 0.0f);
		game->SwitchScene(1);
		return;
	}

	if (!player->IsTransform()) {
		for (UINT i = 0; i < objects.size(); i++)
		{
			LPGAMEOBJECT object = objects[i];
			vector<LPGAMEOBJECT> coObjects;
			if (dynamic_cast<ParaGoomba*>(object))
			{
				ParaGoomba* goomba = dynamic_cast<ParaGoomba*>(object);
				if (goomba->state == PARAGOOMBA_STATE_GOOMBA)
					goomba->ChangeToGoomba(grid);
			}
			if (dynamic_cast<KoopaParaTroopa*>(object))
			{
				KoopaParaTroopa* parakoopa = dynamic_cast<KoopaParaTroopa*>(object);
				if (parakoopa->state == PARATROOPA_STATE_KOOPA)
					parakoopa->ChangeToKoopa(grid);
			}
			GetColliableObjects(object, coObjects);
			object->Update(dt, &coObjects);
		}
	}


	SetInactivation();
	// Update camera to follow mario
	UpdateCameraPosition();
	hud->Update(dt);
	UpdateGrid();
	
}
void PlayScene::GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Item*>(curObj))
	{
		//coObjects.push_back(player);
		for (auto obj : objects)
		{
			if (!dynamic_cast<Enemy*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<PointEffect*>(curObj) || dynamic_cast<HitEffect*>(curObj) ||
		dynamic_cast<BrokenBrickEffect*>(curObj) || dynamic_cast<FirePlantBullet*>(curObj))
	{
		return;
	}
	else if (dynamic_cast<FirePiranhaPlant*>(curObj) || dynamic_cast<PiranhaPlant*>(curObj))
	{
	/*	coObjects.push_back(player);*/
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (!(dynamic_cast<KoopaTroopa*>(obj) && obj->state == KOOPATROOPA_STATE_HIDING))
					continue;
			}
			if (dynamic_cast<Ground*>(obj) || 	dynamic_cast<InvisibleBrick*>(obj) ||
				dynamic_cast<FireBall*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<KoopaTroopa*>(curObj))
	{
	/*	coObjects.push_back(player);*/
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				
				if (dynamic_cast<Enemy*>(obj)->isDead || dynamic_cast<Enemy*>(obj)->IsInactive())
					continue;
			}
			if (!dynamic_cast<FirePlantBullet*>(obj) && !dynamic_cast<Item*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Goomba*>(curObj))
	{
	/*	coObjects.push_back(player);*/
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (!(dynamic_cast<KoopaTroopa*>(obj) && obj->state == KOOPATROOPA_STATE_HIDING))
					continue;
			}
			if (!dynamic_cast<Block*>(obj) && !dynamic_cast<FirePlantBullet*>(obj) && 
				!dynamic_cast<Item*>(obj) )
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<ParaGoomba*>(curObj))
	{
	/*	coObjects.push_back(player);*/
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (!(dynamic_cast<KoopaTroopa*>(obj) && obj->state == KOOPATROOPA_STATE_HIDING))
					continue;
			}
			if (!dynamic_cast<Block*>(obj) && !dynamic_cast<FirePlantBullet*>(obj) &&
				!dynamic_cast<Item*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<KoopaParaTroopa*>(curObj))
	{
		coObjects.push_back(player);
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (!(dynamic_cast<KoopaTroopa*>(obj) && obj->state == KOOPATROOPA_STATE_HIDING))
					continue;
			}
			if (!dynamic_cast<FirePlantBullet*>(obj)&& !dynamic_cast<Item*>(obj) )
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<FireBall*>(curObj)|| dynamic_cast<RaccoonTail*>(curObj))
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj) )
			{
				if (dynamic_cast<Enemy*>(obj)->IsInactive())
					continue;
				else 
					coObjects.push_back(obj);
			}
			if (!dynamic_cast<FirePlantBullet*>(obj) && !dynamic_cast<Item*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Mario*>(curObj))
	{
		for (auto obj : objects)
		{
				if ( dynamic_cast<Ground*>(obj) ||dynamic_cast<Block*>(obj) ||
				 dynamic_cast<Brick*>(obj) || dynamic_cast<Item*>(obj) ||
				dynamic_cast<Pipe*>(obj) || dynamic_cast<Portal*>(obj))
				coObjects.push_back(obj);
			else 
			{
				if (dynamic_cast<FirePlantBullet*>(obj) && obj->IsEnable() == true)
					coObjects.push_back(obj);
				else if ((dynamic_cast<Enemy*>(obj)) && obj->isEnable == true)
				{
					if(!dynamic_cast<Enemy*>(obj)->IsInactive())
						coObjects.push_back(obj);
				}
			}
		}
	}
}

void PlayScene::Render()
{
	Game* game = Game::GetInstance();
	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	float camX = game->GetCamX();
	float camY = game->GetCamY();
	// Khi không ở trong teleport
	if (!player->isInTeleport)
	{
		this->map->Render(camX, camY, screenWidth, screenHeight);
		for (auto obj : listItems)
		{
			if (obj->IsEnable() == false)
				continue;
			obj->Render();
		}
		for (auto obj : listStaticObjectsToRender)
		{
			if (obj->IsEnable() == false)
				continue;
			obj->Render();
		}
		for (auto obj : listMovingObjectsToRender)
		{
			if (obj->IsEnable() == false)
			{
				continue;
			}
			obj->Render();
		}
		player->Render();
		for (auto obj : listPipesToRender)
		{
			if (obj->IsEnable() == false)
				continue;
			obj->Render();
		}
		hud->Render();
	}
	else
	{
		// Khi ở trong teleport thì màn hình chuyển sang màu đen
		// một khoảng thời gian
		LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);
		Game* game = Game::GetInstance();
		Game::GetInstance()->Draw(camX,camY
			, bbox, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
			, 255);
	}
}
void PlayScene::UpdatePlayer(DWORD dt)
{
	if (player->state == MARIO_STATE_DEATH)
	{
		isGameOver = true;
	}
		
	vector<LPGAMEOBJECT> coObjects;
	GetColliableObjects(player, coObjects);
	player->Update(dt, &coObjects);
}
/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];
	for (unsigned int i = 0; i < listUnits.size(); i++)
		delete listUnits[i];

	objects.clear();
	listStaticObjectsToRender.clear();
	listMovingObjectsToRender.clear();
	listPipesToRender.clear();
	listItems.clear();
	listUnits.clear();

	delete map;
	delete hud;

	portal = NULL;
	hud = NULL;
	grid = NULL;
	unit = NULL;
	player = NULL;
	hud = NULL;
	map = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->StartJumping();
		mario->Float();
		mario->Fly();
		break;
	case DIK_U:
		mario->UpForm();
		break;
	case DIK_R:
		mario->Reset();
		break;
	case DIK_I:
		mario->Information();
		break;
	case DIK_DOWN:
		mario->Squat();
		break;
	case DIK_A:
	{
		int flag = mario->Skill();
		if (flag == 1)
		{
			Grid* grid = ((PlayScene*)scence)->GetGrid();
			mario->ShootFireBall(grid);
		}
		if (flag == 2)
		{
			mario->TailAttack();
		}
		break;
	}
	case DIK_Z:
		mario->SetTele(505, 300);
		break;
	case DIK_X:
		mario->SetTele(735, 380);
		break;
	case DIK_C:
		mario->SetTele(1327, 380);
		break;
	case DIK_V:
		mario->SetTele(1930, 380);
		break;
	case DIK_B:
		mario->SetTele(2275, 90);
		break;
	case DIK_N:
		mario->SetTele(2488, 380);
		break;
	case DIK_P:
		((PlayScene*)scence)->GetGrid()->Out();
		break;
	}
}
void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_A:
		mario->TurnOffSkill();
		break;
	case DIK_S:
		mario->Jump();
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_IDLE);
		mario->isSquat = false;
		break;
	}
		
}
void PlayScenceKeyHandler::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DEATH || mario->isAutoWalk ||
		mario->isInTeleport || mario->isTeleport) return;
	if (game->IsKeyDown(DIK_S))
	{
		mario->SuperJump();
	}
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->FillUpPowerMelter();
			mario->PickUp();
		}
		else if (!game->IsKeyDown(DIK_A))
			mario->LosePowerMelter();
		mario->SetWalkingRight();
		mario->Friction();
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->FillUpPowerMelter();
			mario->PickUp();
		}
		else if(!game->IsKeyDown(DIK_A))
			mario->LosePowerMelter();
		mario->SetWalkingLeft();
		mario->Friction();
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		mario->Squat();
	}
	else
	{
		mario->LosePowerMelter();
		mario->Friction();
	}
	
}
//Bật Camera khi Mario bay 
void PlayScene::TurnCamY(float playerY, bool isFlying, int ScreenHeight, int MapHeight)
{
	
	if (isTurnCamY == true && playerY > (map->edgeBottomInWorld + 16) - ScreenHeight/2)
	{
		isTurnCamY = false;
	}
	
	if (isFlying == true)
		isTurnCamY = true;
}
void PlayScene::UpdateCameraPosition()
{
	float centerCamX, centerCamY;
	player->GetPosition(centerCamX, centerCamY);
	Game* game = Game::GetInstance();

	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	int mapWidth = map->GetMapWidth();
	int mapHeight = map->GetMapHeiht();
	float oldCamY = game->GetCamY();

	TurnCamY(centerCamY, player->IsFlying(), screenHeight, mapHeight);

	float camX = map->edgeLeft, camY = oldCamY;
	centerCamX -= screenWidth / 2;
	centerCamY -= screenHeight / 2;
	float edgeBottom = 0;


	if (player->isInExtraMap == false)
		edgeBottom = map->edgeBottomInWorld + HUD_BBOX_HEIGHT;
	else
		edgeBottom = map->edgeBottomInExtraMap + HUD_BBOX_HEIGHT;


	// CamX
	if (player->x > map->edgeLeft + screenWidth / 2)
	{
		camX = centerCamX;
	}
	if (player->x + screenWidth / 2 > mapWidth - map->edgeLeft)
	{
		camX = (float)(mapWidth - map->edgeLeft) - screenWidth;
	}

	// CAMY
	if (player->y + screenHeight > edgeBottom)
	{
		camY = (float)(edgeBottom - screenHeight);
	}
	if (player->y - screenHeight / 2 < map->edgeTop)
		camY = (float)map->edgeTop;
	else
	{
		if (isTurnCamY)
			camY = centerCamY;
	}

	// Cam ở extraMap
	if (player->isInExtraMap)
	{
		camX = map->edgeLeftInExtraMap;
		if (player->x > map->edgeLeftInExtraMap + screenWidth / 2)
		{
			camX = centerCamX;
		}
		if (player->x + screenWidth / 2 > map->edgeRight)
		{
			camX = (float)(map->edgeRightInExtraMap) - screenWidth;
		}
	}
	Game::GetInstance()->SetCamPos(round(camX), round(camY));
};
void PlayScene::GetObjectFromGrid()
{
	listUnits.clear();
	listStaticObjectsToRender.clear();
	listMovingObjectsToRender.clear();
	listPipesToRender.clear();
	listItems.clear();
	objects.clear();

	Game* game = Game::GetInstance();
	float camX,camY;

	camX = game->GetCamX();
	camY = game->GetCamY();

	grid->Get(camX,camY, listUnits);

	for (UINT i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		objects.push_back(obj);

		if (dynamic_cast<Block*>(obj) || dynamic_cast<Ground*>(obj))
			continue;

		else if (dynamic_cast<Brick*>(obj) || dynamic_cast<Portal*>(obj))
			listStaticObjectsToRender.push_back(obj);

		else if (dynamic_cast<Enemy*>(obj) || dynamic_cast<FirePlantBullet*>(obj) ||
			dynamic_cast<FireBall*>(obj))
			listMovingObjectsToRender.push_back(obj);

		else if (dynamic_cast<PointEffect*>(obj) ||	dynamic_cast<HitEffect*>(obj) ||
			dynamic_cast<BrokenBrickEffect*>(obj))
			listMovingObjectsToRender.push_back(obj);


		else if (dynamic_cast<Item*>(obj))
			listItems.push_back(obj);

		else if (dynamic_cast<Pipe*>(obj))
			listPipesToRender.push_back(obj);
	}
 }
void PlayScene::UpdateGrid()
{
	for(unsigned int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		if (obj->IsEnable() == false)
			continue;
		float newPosX, newPosY;

		obj->GetPosition(newPosX, newPosY);
		listUnits[i]->Move(newPosX, newPosY);
	}
}
bool PlayScene::IsInViewport(LPGAMEOBJECT object)
{
	Game* game = Game::GetInstance();
	float camX, camY;
	float objX, objY;

	camX = game->GetCamX();
	camY = game->GetCamY();

	if (camX == 0 && camY == 0)
	{
		UpdateCameraPosition();
		camX = game->GetCamX();
		camY = game->GetCamY();
	}
	
	object->GetPosition(objX, objY);
	
	return objX >= camX  && objX < camX + SCREEN_WIDTH
		&& objY >= camY && objY < camY + SCREEN_HEIGHT;
};

// Inactive các quái khi ra khỏi Viewport
void PlayScene::SetInactivation()
{
	Game* game = Game::GetInstance();
	
	for (auto object : objects)
	{
		if (!IsInViewport(object))
		{
			if (dynamic_cast<Enemy*>(object) )
			{
				dynamic_cast<Enemy*>(object)->Inactive();
				dynamic_cast<Enemy*>(object)->AbleToActive();
			}
			else if (dynamic_cast<FireBall*>(object) || dynamic_cast<PointEffect*>(object) ||
				dynamic_cast<FirePlantBullet*>(object) || dynamic_cast<BrokenBrickEffect*>(object))
			{
				object->isEnable = false;
			}
			else if (dynamic_cast<Item*>(object))
			{
				if (!dynamic_cast<Coin*>(object))
				{
					object->isEnable = false;
				}
				
			}
		}
		else
		{
			if (dynamic_cast<Enemy*>(object))
			{
				dynamic_cast<Enemy*>(object)->isAbleToActive = false;
			}
		}
	};
}


// Active các đối tượng quái khi entryPoint gặp Viewport
void PlayScene::ActiveEnemiesInViewport()
{
	Game* game = Game::GetInstance();
	for (auto obj : objects)
	{
		if (dynamic_cast<Enemy*>(obj))
		{
			Enemy* enemy = dynamic_cast<Enemy*>(obj);
			if (enemy->IsInactive() && enemy->IsEnable())
			{
				if (IsInViewport(enemy) == true
					&& enemy->isAbleToActive == true)
				{
					enemy->Active();
				}
			}
		}
	}
}
void PlayScene::GetExtraMapPosition(float& x, float& y)
{
	x = map->extraMapPositionX;
	y = map->extraMapPositionY;
}
void PlayScene::GetWorldMapPosition(float& x, float& y)
{
	x = map->worldMapPositionX;
	y = map->worldMapPositionY;
}

#include "PlayScene.h"
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

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
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

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new AnimationSet();

	Animations* animations = Animations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
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
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());


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
		obj = new Mario(x, y);
		player = (Mario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new Goomba(x,y); break;
	case OBJECT_TYPE_BRICK: obj = new Brick(); break;
	case OBJECT_TYPE_KOOPAS:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new KoopaTroopa(x, y,set_type);
		break;
	}
	case OBJECT_TYPE_BLOCKS: obj = new Block(); break;
	case OBJECT_TYPE_GROUNDS: obj = new Ground(); break;
	case OBJECT_TYPE_PIPES:	obj = new Pipe(); break;
	case OBJECT_TYPE_INVISIBLEBRICK:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new InvisibleBrick(set_type);
		break;
	}
		
	/*case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}*/
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
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

	this->map = new Map(idMap, tolRowTileSet, tolColTileSet, tolRowMap, tolColMap, totalTiles);
	map->LoadMatrix(MatrixPath.c_str());
	map->CreateTilesFromTileSet();
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

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
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

void PlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	
	for (size_t i = 0; i < objects.size(); i++)
	{
		
		if (dynamic_cast<Enemy*>(objects[i]))
		{
			if (dynamic_cast<Enemy*>(objects[i])->IsAbleToActive() == true)
				objects[i]->Update(dt, &coObjects);
			else
				continue;
		}
		else
			objects[i]->Update(dt, &coObjects);
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;
	

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	Game* game = Game::GetInstance();
	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	int mapWidth = map->GetMapWidth();
	int mapHeight = map->GetMapHeiht();
	int oldCamY = game->GetCamY();

	//cx -= screenWidth /2;
	//cy -= screenHeight /2;
	TurnCamY(cy, player->IsFlying(), screenHeight, mapHeight);
	if (cx - (screenWidth / 2) <= 16)
	{
		cx = 16;
	}
	else if (cx + (screenWidth / 2) > (mapWidth - 16))
	{
		cx = (mapWidth - 16) - screenWidth;
	}	
	else
	{
		cx -= (screenWidth / 2);
	}
	if (_turnCamY == false)
	{
		if (cy + SCREEN_HEIGHT > 448)
		{
			cy = 448 - screenHeight;
		}
		else if (cy - screenHeight < 16)
		{
			cy = 16;
		}
	}
	else
	{
		if (cy - (screenHeight / 2) < 16)
			cy = 16;
		else if (cy + (screenHeight / 2) >= 448)
			cy = 448 - screenHeight;
		else
		{
			cy -= screenHeight / 2;
		}
	}
	Game::GetInstance()->SetCamPos(round(cx), round(cy));
}

void PlayScene::Render()
{
	this->map->Render();
	for (int i = 1; i < objects.size(); i++)
		objects[i]->Render();
	objects[0]->Render();
}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	//LPFireBall fireball = ((PlayScene*)scence)->GetFireBall();
	switch (KeyCode)
	{
	case DIK_K:
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
	case DIK_C:
	{
		KoopaTroopa* koopa = new KoopaTroopa(960, 383, 1);
		koopa->SetEnable();
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(4);
		koopa->SetAnimationSet(ani_set);
		((PlayScene*)scence)->AddObject((KoopaTroopa*)koopa);
		break;
	}
	case DIK_G:
	{
		Goomba* goopa = new Goomba(960, 383);
		goopa->SetEnable();
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(3);
		goopa->SetAnimationSet(ani_set);
		((PlayScene*)scence)->AddObject((Goomba*)goopa);
		break;
	}
	case DIK_I:
		mario->Information();
		break;
	case DIK_J:
		int flag = mario->Skill();
	
		if (flag == 1)
		{
			GameObject* fireBall = mario->ShootFireBall();
			((PlayScene*)scence)->AddObject((FireBall*)fireBall);
		}
		if (flag == 2)
		{
			mario->TailAttack();
		}
			
		break;
	//case DIK_C:
	//	CreateKoopa();
	//	break;
		
	}
}
void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_J:
		mario->LosePowerMelter();
		mario->ReleaseJ();
		/*mario->SetState(MARIO_STATE_IDLE);*/
	/*	mario->SetState(MARIO_STATE_IDLE);*/
		break;
	case DIK_A:
		mario->turnFriction = true;
		break;
	case DIK_D:
		mario->turnFriction = true;
		break;
	case DIK_K:
	/*	mario->Float();*/
		mario->Jump();
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_IDLE);
	}
		
}
void PlayScenceKeyHandler::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DEATH) return;
	if (game->IsKeyDown(DIK_D))
	{
		mario->SetDirect(true);
		if (!mario->IsFlying() && !mario->IsFloating())
		{
			if (game->IsKeyDown(DIK_J))
			{
				mario->FillUpPowerMelter();
				mario->PickUp();
			}

			mario->SetState(MARIO_STATE_WALKING);
			if (game->IsKeyDown(DIK_A))
			{
				mario->SetDirect(false);
				mario->SetState(MARIO_STATE_BRAKING);
			}
			if (game->IsKeyDown(DIK_K))
			{
				mario->SuperJump();
			}
		}
		
	}
	else if (game->IsKeyDown(DIK_A))
	{
		mario->SetDirect(false);
		if (!mario->IsFlying() && !mario->IsFloating())
		{
			if (game->IsKeyDown(DIK_J))
			{
				mario->FillUpPowerMelter();
				mario->PickUp();
			}
			mario->SetState(MARIO_STATE_WALKING);
			if (game->IsKeyDown(DIK_D))
			{
				mario->SetDirect(true);
				mario->SetState(MARIO_STATE_BRAKING);
			}
			if (game->IsKeyDown(DIK_K))
			{
				mario->SuperJump();
			}
		}
		
	}
	//else if (game->IsKeyDown(DIK_K))
	//{
	//	/*mario->Jump();*/
	//}
	else if (game->IsKeyDown(DIK_S))
	{
		mario->Squat();
	}
	else if (game->IsKeyDown(DIK_K))
	{
		mario->SuperJump();
	}
	else
	{
	
	/*	mario->SetState(MARIO_STATE_IDLE);*/
	/*	mario->turnFriction = true;*/
		if (game->IsKeyDown(DIK_J))
		{
			mario->PickUp();
		}
	}
	
}
void PlayScene::AddObject(GameObject* obj)
{
	this->objects.push_back(obj);
	DebugOut(L"Size: %d", this->objects.size());
}
void PlayScene::TurnCamY(float _playerY, bool isFlying, int ScreenHeight, int MapHeight)
{
	if (_turnCamY == true && _playerY > 448 - ScreenHeight/2
		)
	{
		if (isFlying != true)
			_turnCamY = false;
		else
			_turnCamY = true;
	}
	
	if (isFlying == true)
		_turnCamY = true;
}

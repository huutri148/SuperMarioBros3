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

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

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

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

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
		break;
	}
	case OBJECT_TYPE_KOOPAS:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new KoopaTroopa(x, y, set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_BLOCKS: 
	{
		float width = (float)atof(tokens[4].c_str());
		float height = (float)atof(tokens[5].c_str());
		obj = new Block(x,y,width,height); 
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_GROUNDS:
	{
		float width = (float)atof(tokens[4].c_str());
		float height = (float)atof(tokens[5].c_str());
		obj = new Ground(x ,y, width, height);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_PIPES: 
	{
		obj = new Pipe();
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_INVISIBLEBRICK:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new InvisibleBrick(set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new PiranhaPlant(x, y, set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	case OBJECT_TYPE_FIREPIRANHAPLANT:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new FirePiranhaPlant(x, y, set_type);
		unit = new Unit(grid, obj, x, y);
		break;
	}
	/*case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}*/
	case OBJECT_TYPE_COIN:
	{
		int set_type = atoi(tokens[4].c_str());
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
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
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

	//DebugOut(L"\nSize of obj: %d", objects.size());
	for (UINT i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT object = objects[i];
		vector<LPGAMEOBJECT> coObjects;
		if (dynamic_cast<Enemy*>(object))
			dynamic_cast<Enemy*>(object)->GetPlayerPosition(player->x, player->y);
		if (dynamic_cast<FirePiranhaPlant*>(object))
		{
			FirePiranhaPlant* plant = dynamic_cast<FirePiranhaPlant*>(object);
			if (plant->canShoot == true)
			{
				plant->Shooting(grid);
			}
		}
		else if (dynamic_cast<Brick*>(object))
		{
			Brick* brick = dynamic_cast<Brick*>(object);
			if (brick->isUsed == true )
				brick->DropItem(grid);
		}
		else if (dynamic_cast<ParaGoomba*>(object))
		{
			ParaGoomba* goomba = dynamic_cast<ParaGoomba*>(object);
			if (goomba->state == PARAGOOMBA_STATE_GOOMBA)
				goomba->ChangeToGoomba(grid);
		}
		else if (dynamic_cast<KoopaParaTroopa*>(object))
		{
			KoopaParaTroopa* parakoopa = dynamic_cast<KoopaParaTroopa*>(object);
			if (parakoopa->state == PARATROOPA_STATE_KOOPA)
				parakoopa->ChangeToKoopa(grid);
		}
		GetColliableObjects(object, coObjects);
		object->Update(dt, &coObjects);
	}

	// Update camera to follow mario
	SetInactivation();

	UpdateCameraPosition();
	hud->Update(dt);
	UpdateGrid();
	
}
void PlayScene::GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Item*>(curObj))
	{
		for (auto obj : objects)
		{
			if (!dynamic_cast<Enemy*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<FirePiranhaPlant*>(curObj) || 
		dynamic_cast<PiranhaPlant*>(curObj))
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (!dynamic_cast<KoopaTroopa*>(obj))
					continue;
			}
			if (dynamic_cast<Ground*>(obj) || 	dynamic_cast<InvisibleBrick*>(obj) ||
				dynamic_cast<FireBall*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<FirePlantBullet*>(curObj))
	{
		return;
	}
	else if (dynamic_cast<KoopaTroopa*>(curObj))
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (dynamic_cast<Enemy*>(obj)->isDead == true ||
					dynamic_cast<Enemy*>(obj)->IsInactive())
					continue;
			}
			if (!dynamic_cast<FirePlantBullet*>(obj) && !dynamic_cast<Item*>(obj)
				&& !dynamic_cast<InvisibleBrick*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Goomba*>(curObj))
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				// tránh trường hợp quái rớt trúng đầu và bị rớt xuống
				if (!dynamic_cast<KoopaTroopa*>(obj))
					continue;
			}
			if (!dynamic_cast<Block*>(obj) && !dynamic_cast<FirePlantBullet*>(obj) 
				&&!dynamic_cast<Item*>(obj) && !dynamic_cast<InvisibleBrick*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<ParaGoomba*>(curObj))
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (!dynamic_cast<KoopaTroopa*>(obj))
					continue;
			}
			if (!dynamic_cast<Block*>(obj) && !dynamic_cast<FirePlantBullet*>(obj)
				&& !dynamic_cast<Item*>(obj) && !dynamic_cast<InvisibleBrick*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<KoopaParaTroopa*>(curObj))
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (!dynamic_cast<KoopaTroopa*>(obj))
					continue;
			}
			if (!dynamic_cast<FirePlantBullet*>(obj)&& !dynamic_cast<Item*>(obj) 
				&& !dynamic_cast<InvisibleBrick*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<FireBall*>(curObj))
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Enemy*>(obj))
			{
				if (dynamic_cast<Enemy*>(obj)->IsInactive())
					continue;
				else 
					coObjects.push_back(obj);
			}
			if (!dynamic_cast<FirePlantBullet*>(obj) && !dynamic_cast<Item*>(obj)
				&& !dynamic_cast<InvisibleBrick*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Mario*>(curObj))
	{
		for (auto obj : objects)
		{
				if ( dynamic_cast<Ground*>(obj) ||dynamic_cast<Block*>(obj)
				|| dynamic_cast<Brick*>(obj) || dynamic_cast<Item*>(obj) ||
				dynamic_cast<Pipe*>(obj))
				coObjects.push_back(obj);
			else 
			{
				if (dynamic_cast<FirePlantBullet*>(obj) && obj->IsEnable() == true)
					coObjects.push_back(obj);
				else if ((dynamic_cast<Enemy*>(obj))
					&& obj->isEnable == true)
					coObjects.push_back(obj);
			}
		}
	}
}

void PlayScene::Render()
{
	Game* game = Game::GetInstance();

	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	float cam_x = game->GetCamX();
	float cam_y = game->GetCamY();

	//DebugOut(L"\nListMoving Obj: %d", listMovingObjectsToRender.size());
	this->map->Render(cam_x,cam_y,screenWidth,screenHeight);
	for (auto obj : listMovingObjectsToRender)
	{
		if (obj->IsEnable() == false || obj == NULL)
		{
			continue;
		}
			
		obj->Render();
	}
	
	for (auto obj : listStaticObjectsToRender)
	{
		if (obj->IsEnable() == false)
			continue;
		
		obj->Render();
	}
	player->Render();
	hud->Render();
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

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
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
	case DIK_I:
		mario->Information();
		break;
	case DIK_J:
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
}
void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_J:
		mario->ReleaseJ();
		break;
	case DIK_A:
		mario->turnFriction = true;
		break;
	case DIK_D:
		mario->turnFriction = true;
		break;
	case DIK_K:
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
			if (game->IsKeyDown(DIK_K))
			{
				mario->SuperJump();
			}
		}
		
	}
	if (game->IsKeyDown(DIK_A))
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
			if (game->IsKeyDown(DIK_K))
			{
				mario->SuperJump();
			}
		}
		
	}
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
		mario->LosePowerMelter();
		if (game->IsKeyDown(DIK_J))
		{
			mario->PickUp();
		}
	}
	
}
//Bật Camera khi Mario bay 
void PlayScene::TurnCamY(float _playerY, bool isFlying, int ScreenHeight, int MapHeight)
{
	if (isTurnCamY == true && _playerY > 448 - ScreenHeight/2)
	{
		isTurnCamY = false;
	}
	
	if (isFlying == true)
		isTurnCamY = true;
}
void PlayScene::UpdateCameraPosition()
{
	float cx, cy;
	player->GetPosition(cx, cy);
	Game* game = Game::GetInstance();
	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	int mapWidth = map->GetMapWidth();
	int mapHeight = map->GetMapHeiht();
	float oldCamY = game->GetCamY();

	TurnCamY(cy, player->IsFlying(), screenHeight, mapHeight);
	float Sx = 16, Sy = oldCamY;
	cx -= screenWidth / 2;
	cy -= screenHeight / 2;
	// Ở đây xét số cứng 
	// Cần phải sửa vì còn extra map ở dưới
	// nếu có thể chia thành hai map thì sẽ để theo 
	//chiều dài và chiều rộng của map
	if (player->x > 16 + screenWidth / 2)
	{
		Sx = cx;
	}
	if (player->x + screenWidth / 2 > mapWidth - 16)
	{
		Sx = (float)(mapWidth - 16.0f) - screenWidth;
	}
	if (player->y + screenHeight > 490)
	{
		Sy = (float)(490.0f - screenHeight);
	}
	if (player->y - screenHeight / 2 < 16)
		Sy = (float)16.0f;
	else
	{
		if (isTurnCamY)
			Sy = cy;
	}
	if(player->isSwingTail == false)
		Game::GetInstance()->SetCamPos(round(Sx), round(Sy));
};
void PlayScene::GetObjectFromGrid()
{
	listUnits.clear();
	listStaticObjectsToRender.clear();
	listMovingObjectsToRender.clear();
	/*listPipeToRender.clear();*/
	objects.clear();

	Game* game = Game::GetInstance();
	float cam_x,cam_y;

	cam_x = game->GetCamX();
	cam_y = game->GetCamY();

	grid->Get(cam_x,cam_y, listUnits);

	for (UINT i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		objects.push_back(obj);

		if (  dynamic_cast<Block*>(obj) || dynamic_cast<Ground*>(obj)||
			dynamic_cast<InvisibleBrick*>(obj))
			continue;
		else if (dynamic_cast<Brick*>(obj)|| dynamic_cast<Pipe*>(obj))
			listStaticObjectsToRender.push_back(obj);
	/*	else if (dynamic_cast<Pipe*>(obj))
			listPipeToRender.push_back(obj);*/
		else if (dynamic_cast<Enemy*>(obj)|| dynamic_cast<FirePlantBullet*>(obj) ||
			dynamic_cast<FireBall*>(obj))
			listMovingObjectsToRender.push_back(obj);
		else if (dynamic_cast<Item*>(obj))
			listMovingObjectsToRender.push_back(obj);
	}
 }
void PlayScene::UpdateGrid()
{
	for(unsigned int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		if (obj->IsEnable() == false)
			continue;
		float newPos_x, newPos_y;
		obj->GetPosition(newPos_x, newPos_y);
		listUnits[i]->Move(newPos_x, newPos_y);
	}
}
bool PlayScene::IsInViewport(LPGAMEOBJECT object)
{
	Game* game = Game::GetInstance();
	float cam_x, cam_y;
	cam_x =game->GetCamX();
	cam_y = game->GetCamY();
	if (cam_x == 0 && cam_y == 0)
		UpdateCameraPosition();
	float obj_x, obj_y;
	object->GetPosition(obj_x, obj_y);
	
	return obj_x >= cam_x && obj_x < cam_x + SCREEN_WIDTH
		&& obj_y >= cam_y && obj_y < cam_y + SCREEN_HEIGHT;
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
		/*	else if (dynamic_cast<Item*>(object) && object->IsEnable() == true)
			{
				dynamic_cast<Item*>(object)->SetEnable(false);
			}*/
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
					enemy->Active();
			}
		}
	}
}

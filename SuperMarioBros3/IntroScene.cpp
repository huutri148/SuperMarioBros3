#include "IntroScene.h"
#include"Player.h"


using namespace std;

IntroScene::IntroScene(int id, LPCWSTR filePath) 
	: Scene(id, filePath)
{
	key_handler = new IntroSceneKeyHandler(this);
}

void IntroScene::_ParseSection_TEXTURES(string line)
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

void IntroScene::_ParseSection_SPRITES(string line)
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

void IntroScene::_ParseSection_ANIMATIONS(string line)
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

void IntroScene::_ParseSection_ANIMATION_SETS(string line)
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
void IntroScene::_ParseSection_OBJECTS(string line)
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
	case OBJECT_TYPE_MARIO:
		obj = new Mario();
		mario = (Mario*)obj;
		mario->SetDirect(false);
		mario->SetLevel(MARIO_BIG_FORM);
		mario->isInIntroScene = true;
		mario->isEnable = false;
		DebugOut(L"[INFO] Player1 object created!\n");
		break;
	case OBJECT_TYPE_LUGI:
		obj = new Mario();
		lugi = (Mario*)obj;	
		lugi->isInIntroScene = true;
		lugi->SetLevel(MARIO_BIG_FORM);
		lugi->isEnable = false;
		DebugOut(L"[INFO] Player2 object created!\n");
		break;
	case OBJECT_TYPE_GROUNDS:
	{
		float width = (float)atof(tokens[4].c_str());
		float height = (float)atof(tokens[5].c_str());
		obj = new Ground( width, height);
		break;
	}
	case OBJECT_TYPE_GOOMBA:
		obj = new Goomba(x,y);
		goomba = (Goomba*)obj;
		obj->isEnable = false;
		obj->vx = 0;
		fallingObjects.push_back(obj);
		break;
	case OBJECT_TYPE_RACCOONLEAF:
		obj = new RaccoonLeaf(x, y);
		fallingObjects.push_back(obj);
		break;
	case OBJECT_TYPE_MUSHROOM:
		obj = new Mushroom();
		obj->isEnable = false;
		fallingObjects.push_back(obj);
		break;
	case OBJECT_TYPE_SHELL:
		obj = new KoopaTroopa();
		((KoopaTroopa*)obj)->SetState(KOOPATROOPA_STATE_HIDING);
		((KoopaTroopa*)obj)->isEnable = false;
		((KoopaTroopa*)obj)->forceShell = true;
		if (koopa == NULL)
			koopa = (KoopaTroopa*)obj;
		fallingObjects.push_back(obj);
		break;
	case OBJECT_TYPE_BACKGROUND_INTRO_SCENE:
		obj = new BackGroundIntroScene();
		backGround = (BackGroundIntroScene*)obj;
		DebugOut(L"[INFO] BackGround  object created!\n");
		break;
	case OBJECT_TYPE_KOOPAS:
		obj = new KoopaTroopa();
		koopas.push_back(obj);
		obj->nx = 1;
		obj->vx = 0.03f;
		if (koopas.size() == 4)
			obj->vx = 0.05f;
		obj->isEnable = false;
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

void IntroScene::Load()
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
		}
	}

	f.close();

	Textures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void IntroScene::Update(DWORD dt)
{
	if (mario == NULL || lugi == NULL)
		return;

	// Đàn rùa ở cuối cảnh
	ShowKoopas();

	HandleBehaviourOfMario();

	HandleBehaviourOfLugi();


	// SECTION1 của IntroScene
	if (backGround->isBeginSection1 == true)
	{
		mario->isEnable = true;
		lugi->isEnable = true;
		// Khi Intro vừa bắt đầu thì MARIO và LUGI ở trạng thái Run nhưng
		// tọa độ không thay đổi
		lugi->SetAutoWalk(1, 0.0f);
		mario->SetAutoWalk(-1, 0.0f);
	}

	// SECTION2 của Intro Scene
	if (backGround->isBeginSection2 == true)
	{
		if (mario->vx == 0 && lugi->vx == 0)
		{
			lugi->SetAutoWalk(1, MARIO_WALKING_SPEED);
			mario->SetAutoWalk(-1, -MARIO_WALKING_SPEED);
		}
		else if (GetTickCount() -backGround->beginSection2Time > SECTION2_TIME &&
			backGround->beginSection2Time != 0)
		{
			lugi->SetAutoJump(-MARIO_SUPER_JUMP_SPEED);
			lugi->isAutoWalk = false;
			backGround->beginSection2Time = 0;
		}
		else
		{
			// LUGI nhảy đụng vào góc trên màn hình sẽ bật cờ để 
			// Section 3 bắt đầu
			if (lugi->y < 30)
			{
				backGround->isBeginSection2 = false;
				backGround->isBeginSection3 = true;
				mario->SetState(MARIO_STATE_IDLE);
			}
		}
	}

	// SECTION3 của IntroScene
	if (backGround->isBeginSection3 == true &&
		backGround->isDoneSection3 == false)
	{
		if (GetTickCount() - backGround->section3Time > SECTION3_TIME
			&& backGround->section3Time != 0)
		{
			for (size_t i = 0; i < fallingObjects.size(); i++)
			{
				fallingObjects[i]->isEnable = true;
			}
		}
	}

	mario->Update(dt, &objects);

	// Nếu mario ở trạng thái transform 
	// dừng cập nhật các đội tượng còn lại
	if (!mario->IsTransform())
	{
		backGround->Update(dt, &objects);
		for (size_t i = 5; i < objects.size(); i++)
		{
			if (objects[i]->isEnable == true)
				objects[i]->Update(dt, &objects);
		}
	}


}
void IntroScene::ShowBush()
{
	showingBush = true;
	if (bushsSprite.size() == 0)
	{
		Sprites* sprites = Sprites::GetInstance();
		bushsSprite.push_back(sprites->Get(SPRITE_BUSH1_ID));
		bushsSprite.push_back(sprites->Get(SPRITE_BUSH2_ID));
	}
}

void IntroScene::ShowMenu()
{
	showingMenu = true;
	if (menuSprites.size() == 0)
	{
		Sprites* sprites = Sprites::GetInstance();
		menuSprites.push_back(sprites->Get(SPRITE_MENU_PICKED_PLAYER1_ID));
		menuSprites.push_back(sprites->Get(SPRITE_MENU_PICKED_PLAYER2_ID));
	}
}
void IntroScene::SwitchMenu()
{
	indexMenu++;
	if(indexMenu >= menuSprites.size())
		indexMenu = 0;
}
void IntroScene::ShowKoopas()
{
	if (koopas[0]->x > -30 && koopas[0]->isEnable == true)
	{
		// Mario bắt đầu chạy ngay khi rùa xuất hiện
		mario->SetAutoWalk(1, MARIO_WALKING_SPEED);
		ShowMenu();
		ShowBush();
	}
	if (showingKoopas == true)
	{
		for (size_t i = 0; i < koopas.size(); i++)
		{
			koopas[i]->isEnable = true;
		}
	}
	if (koopas[0]->x > 32 && koopas[0]->isEnable == true)
	{
		showingBush = false;
	}
}


void IntroScene::HandleBehaviourOfMario()
{
	// Chuyển trạng thái khi bị Shell đụng đầu thành nhìn lên trời
	if (mario->isHitted == true)
	{
		if (changeAniTime == 0)
			changeAniTime = GetTickCount();
		else if (GetTickCount() - changeAniTime > CHANGE_STATE_TIME)
		{
			mario->isHitted = false;
			mario->isLookUp = true;
			changeAniTime = GetTickCount();
		}
	}
	if (mario->isLookUp == true)
	{
		if (GetTickCount() - changeAniTime > 800)
		{
			mario->SetAutoJump(-MARIO_SUPER_JUMP_SPEED * 1.5f);
			mario->isLookUp = false;
		}
	}
	if (mario->form == MARIO_RACCOON_FORM &&
		mario->IsInGround() == false)
	{
		if (mario->x > goomba->x && !goomba->isDead)
		{
			mario->Float();
			goomba->SetState(GOOMBA_STATE_WALKING);
			mario->vx = -0.05f;
		}
	}
	// Bật cờ cho sự kiện Đàn Koopas xuất hiện
	if (koopa->x > SCREEN_HEIGHT && marioPickShell == false
		&& mario->form == MARIO_SMALL_FORM && koopa->isEnable == true)
	{
		if (mario->x < SCREEN_HEIGHT - MARIO_SMALL_BBOX_WIDTH - BUSH2_BBOX_WIDTH / 2)
		{
			mario->SetAutoWalk(1, 0.08f);
			mario->powerMelterStack = POWER_MELTER_FULL;
		}
		else
		{
			/// <summary>
			///Chỗ này phải cập nhật lại brake vì brake lỗi 
			/// không nhạy cả khi sử dụng phím và tự chỉnh biến cờ
			/// </summary>
			mario->powerMelterStack = 0;
			mario->SetWalkingLeft();
			koopa->isEnable = false;
			showingKoopas = true;
		}
	}
	if (canJumpToCounter == true)
	{
		if (mario->x + 30 < koopa->x
			&& koopa->x < SCREEN_HEIGHT / 2)
		{
			mario->isAutoWalk = false;
			mario->vx = -0.02f;
			mario->SetAutoJump(-MARIO_JUMP_SPEED_Y);
			canJumpToCounter = false;
			marioPickShell = true;
		}
	}
	// Sự kiện Mario cầm mai rùa để đẩy
	if (marioPickShell == true)
	{
		if (mario->vy == 0)
		{
			mario->SetAutoWalk(1, 0.1f);
			mario->useSkill = true;
		}
		if (mario->x > SCREEN_HEIGHT / 2 - MARIO_BIG_BBOX_WIDTH)
		{
			mario->SetState(MARIO_STATE_IDLE);
			mario->useSkill = false;
			lugi->SetAutoWalk(1, 0.1f);
		}
		if (koopa->x > SCREEN_HEIGHT)
		{
			koopa->x = 16;
			mario->turnToSmallForm = true;
			marioPickShell = false;
		}
	}
}


void IntroScene::HandleBehaviourOfLugi()
{
	// Bật cờ cho lugi cầm mai rùa
	if (goomba->isDead && goomba->isEnable == true)
	{
		if (mario->vx < 0)
		{
			mario->SetAutoWalk(1, 0.06f);
		}
		lugi->SetPosition(SCREEN_HEIGHT, 160);
		lugi->SetAutoWalk(-1, 0.0f);
		lugi->useSkill = true;
	}

	// Sự kiện Lugi cầm mai rùa
	if (lugi->IsPickingShell())
	{
		lugi->SetAutoWalk(-1, -0.08f);
		mario->SetState(MARIO_STATE_IDLE);
		if (lugi->x < SCREEN_WIDTH - BUSH2_BBOX_WIDTH - MARIO_BIG_BBOX_WIDTH)
		{
			lugi->useSkill = false;
			lugi->SetState(MARIO_STATE_IDLE);
			mario->SetAutoWalk(-1, -0.1f);
			canJumpToCounter = true;
		}
	}
}


void IntroScene::Render()
{
	backGround->Render();
	if (showingMenu == true)
		menuSprites.at(indexMenu)->Draw(-1, 16 + BACKGROUND_BBOX_WIDTH/2 - MENU_BBOX_WIDTH /2,
			SCREEN_WIDTH / 2);
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->x >= 16 && objects[i]->x < 258)
		{
			if (objects[i]->isEnable == true)
				objects[i]->Render();
		}
	}
	if (showingBush == true)
	{
		bushsSprite.at(1)->Draw(-1, 209, 101);
	}
}

Mario* IntroScene::GetHoldingPlayer()
{
	if (lugi->IsPickingShell())
		return lugi;
	if (mario->IsPickingShell())
		return mario;
	DebugOut(L"\nNo one holding shell!, it's a bug here");
	return NULL;
}
/*
	Unload current scene
*/
void IntroScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void IntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	IntroScene* scene = (IntroScene*)this->scence;
	switch (KeyCode)
	{
	case DIK_A:
		scene->SwitchMenu();
		break;
	case DIK_S:
		Game* game = Game::GetInstance();
		game->SwitchScene(0);
		Player* p1 = Player::GetInstance();
		break;
	}
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
}
void IntroSceneKeyHandler::OnKeyUp(int KeyCode)
{

}
void IntroSceneKeyHandler::KeyState(BYTE * states)
{
}

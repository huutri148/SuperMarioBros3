#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "KoopaTroopa.h"

#define WINDOW_CLASS_NAME L"SuperMarioBros3"
#define MAIN_WINDOW_TITLE L"Super Mario Bros 3"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(181, 236, 243)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 120

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

CGame* game;

CMario* mario;
CGoomba* goomba;
CKoopaTroopa* koopatroopa;
vector<LPGAMEOBJECT> objects;
float x, y;
class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	//case DIK_SPACE:
	//	mario->SetState(MARIO_STATE_JUMP);
	//	break;
	case DIK_R: // reset
		mario->SetState(MARIO_STATE_IDLE);
		mario->SetLevel(MARIO_BIG_FORM);
		mario->SetPosition(50.0f, 0.0f);
		mario->SetSpeed(0, 0);
		break;
	case DIK_U:
		mario->UpForm();
		break;
	case DIK_I:
		mario->Information();
		break;
	case DIK_SPACE:
		mario->StartJumping();
		break;
	case DIK_C:
		koopatroopa = new CKoopaTroopa();
		koopatroopa->AddAnimation(901);
		koopatroopa->AddAnimation(902);
		koopatroopa->SetPosition(295.0f, 0);
		koopatroopa->SetState(KOOPATROOPA_STATE_WALKING);
		objects.push_back(koopatroopa);
		break;
	case DIK_G:
		goomba = new CGoomba();
		goomba->AddAnimation(801);
		goomba->AddAnimation(802);
		goomba->SetPosition(400 +  60, 135);
		goomba->SetState(GOOMBA_STATE_WALKING);
		objects.push_back(goomba);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	switch(KeyCode)
	{
	case DIK_SPACE:
		mario->unJump();
		break;
	case DIK_B:
		mario->isPickingUp = false;
		DebugOut(L"Release B");
		break;
	
	}
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_D))
	{
		if (game->IsKeyDown(DIK_LSHIFT))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			mario->FillUpPowerMelter();
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_BRAKE_RIGHT);
	}

	else if (game->IsKeyDown(DIK_A))
	{
		if (game->IsKeyDown(DIK_LSHIFT))
		{
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			mario->FillUpPowerMelter();
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		if (game->IsKeyDown(DIK_D))
		{
			mario->SetState(MARIO_STATE_BRAKE_LEFT);
		}
	}
	else if (game->IsKeyDown(DIK_SPACE))
		mario->Jump();
	else if (game->IsKeyDown(DIK_B))
		mario->PickUp();
	//	mario->pickUp = true;
	else
	{
		mario->SetState(MARIO_STATE_IDLE);
		mario->LosePowerMelter();
	}
		

}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, L"textures\\mario.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));


	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);

	// big
	sprites->Add(10001, 246, 154, 260, 181, texMario);		// idle right

	sprites->Add(10002, 275, 154, 290, 181, texMario);		// walking right
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	sprites->Add(10004, 334, 154, 353, 181, texMario);		//Running right
	sprites->Add(10005, 362, 154, 381, 181, texMario);
	sprites->Add(10006, 392, 155, 411, 181, texMario);
	sprites->Add(10007, 425, 154, 441, 182, texMario);		//Brake right


	sprites->Add(10011, 186, 154, 200, 181, texMario);		// idle left
	sprites->Add(10012, 155, 154, 170, 181, texMario);		// walking left
	sprites->Add(10013, 125, 154, 140, 181, texMario);

	sprites->Add(10014, 93, 154, 112, 181, texMario);		//running left
	sprites->Add(10015, 65, 154, 84, 181, texMario);
	sprites->Add(10016, 35, 155, 54, 181, texMario);
	sprites->Add(10017, 5, 154, 21, 182, texMario);			//brake left
	sprites->Add(10099, 215, 120, 231, 135, texMario);		// die 

	// small
	sprites->Add(10021, 247, 0, 259, 15, texMario);			// idle small right
	sprites->Add(10022, 275, 0, 291, 15, texMario);			// walking right
	sprites->Add(10023, 306, 0, 320, 15, texMario);			// 

	sprites->Add(10024, 335, 0, 351, 16, texMario);			//running right

	sprites->Add(10025, 426, 0, 440, 16, texMario);			//brake right

	sprites->Add(10031, 187, 0, 198, 15, texMario);			// idle small left

	sprites->Add(10032, 155, 0, 170, 15, texMario);			// walking left
	sprites->Add(10033, 125, 0, 139, 15, texMario);			// 

	sprites->Add(10034, 95, 0, 111, 16, texMario);			//running left
	sprites->Add(10035, 6, 0, 20, 16, texMario);			//brake left

	//Fire
	sprites->Add(10041, 246, 394, 260, 421, texMario);// idle right
	sprites->Add(10042, 275, 394, 291, 421, texMario);//walking right
	sprites->Add(10043, 305, 395, 321, 421, texMario);

	sprites->Add(10044, 334, 394, 353, 421, texMario);//running right
	sprites->Add(10045, 362, 394, 381, 421, texMario);
	sprites->Add(10046, 392, 395, 411, 421, texMario);

	sprites->Add(10047, 425, 394, 441, 422, texMario);//brake right


	sprites->Add(10051, 186, 394, 200, 421, texMario);//idle left
	sprites->Add(10052, 155, 394, 171, 421, texMario);//walking left
	sprites->Add(10053, 125, 395, 141, 421, texMario);

	sprites->Add(10054, 93, 394, 112, 421, texMario);//running left
	sprites->Add(10055, 65, 394, 84, 421, texMario);
	sprites->Add(10056, 35, 395, 54, 421, texMario);
	sprites->Add(10057, 5, 394, 21, 422, texMario);//brake left

	//RACCOON 
	sprites->Add(10061, 243, 634, 264, 662, texMario); //idle right
	sprites->Add(10062, 272, 634, 294, 662, texMario); //walking right
	sprites->Add(10063, 303, 634, 324, 662, texMario); 

	sprites->Add(10064, 331, 634, 355, 662, texMario);//running right
	sprites->Add(10065, 362, 634, 385, 662, texMario);
	sprites->Add(10066, 393, 634, 414, 662, texMario);

	sprites->Add(10067, 425, 633, 441, 663, texMario);//brake right


	sprites->Add(10071, 182, 634, 203, 662, texMario);//idle left
	sprites->Add(10072, 152, 634, 174, 662, texMario); //walking left
	sprites->Add(10073, 122, 634, 143, 662, texMario);

	sprites->Add(10074, 91, 634, 115, 662, texMario);//running left
	sprites->Add(10075, 61 , 634,84,662, texMario);
	sprites->Add(10076, 32, 634, 58, 662, texMario);

	sprites->Add(10077, 5, 633, 21, 663, texMario);//brake left

	//brick
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 408, 225, 424, 241, texMisc);
	//Goomba
	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);//walk sprite
	sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	sprites->Add(30002, 25, 14, 41, 29, texEnemy);

	sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite
	
	//KoopaTroopa
	sprites->Add(40001, 6, 130, 22, 156, texEnemy); //walk sprite
	sprites->Add(40002, 28, 129, 44, 156, texEnemy);

	sprites->Add(40003, 50, 139, 66, 156, texEnemy);//hiding sprite

	LPANIMATION ani;
	// STATE == IDLE
	ani = new CAnimation(100);	// idle big right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// idle big left
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	// idle small right
	ani->Add(10021);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// idle small left
	ani->Add(10031);
	animations->Add(403, ani);

	ani = new CAnimation(100); // idle fire right
	ani->Add(10041);
	animations->Add(404, ani);

	ani = new CAnimation(100); //idle fire left
	ani->Add(10051);
	animations->Add(405, ani);

	ani = new CAnimation(100); //idle right  racccoon 
	ani->Add(10061);
	animations->Add(406, ani);

	ani = new CAnimation(100);//idle left raccoon
	ani->Add(10071);
	animations->Add(407, ani);


	// WALKING
	ani = new CAnimation(100);	// walk right big
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);	// // walk left big
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	ani = new CAnimation(100);	// walk right small
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	animations->Add(502, ani);

	ani = new CAnimation(100);	// walk left small
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(503, ani);

	ani = new CAnimation(100); //walk right fire
	ani->Add(10041);
	ani->Add(10042);
	ani->Add(10043);
	animations->Add(504, ani);


	ani = new CAnimation(100);//walk left fire
	ani->Add(10051);
	ani->Add(10052);
	ani->Add(10053);
	animations->Add(505, ani);

	ani = new CAnimation(100);//walk left raccoon
	ani->Add(10061);
	ani->Add(10062);
	ani->Add(10063);
	animations->Add(506, ani);

	ani = new CAnimation(100);//walk right raccoon
	ani->Add(10071);
	ani->Add(10072);
	ani->Add(10073);
	animations->Add(507, ani);

	ani = new CAnimation(1000);//big right brake
	ani->Add(10007);
	animations->Add(650, ani);

	ani = new CAnimation(1000);//big left brake
	ani->Add(10017);
	animations->Add(651, ani);

	ani = new CAnimation(100);//small right brake
	ani->Add(10025);
	animations->Add(652, ani);

	ani = new CAnimation(100);//small left brake
	ani->Add(10035);
	animations->Add(653, ani);

	ani = new CAnimation(100);//fire right brake
	ani->Add(10047);
	animations->Add(654, ani);

	ani = new CAnimation(100);//fire left brake
	ani->Add(10057);
	animations->Add(655, ani);

	ani = new CAnimation(100);//raccoon right brake
	ani->Add(10067);
	animations->Add(656, ani);

	ani = new CAnimation(100);//raccoon left brake
	ani->Add(10077);
	animations->Add(657, ani);





	ani = new CAnimation(100);		// Mario die
	ani->Add(10099);
	animations->Add(699, ani);

	ani = new CAnimation(10);  //big running right
	ani->Add(10004);
	ani->Add(10005);
	ani->Add(10006);
	animations->Add(600, ani);



	ani = new CAnimation(10); //big running left
	ani->Add(10014);
	ani->Add(10015);
	ani->Add(10016);
	animations->Add(601, ani);

	ani = new CAnimation(50); //small running right
	ani->Add(10022);
	ani->Add(10023);
	ani->Add(10024);
	animations->Add(602, ani);

	ani = new CAnimation(50);//small running left
	ani->Add(10032);
	ani->Add(10033);
	ani->Add(10034);
	animations->Add(603, ani);

	ani = new CAnimation(50);//Fire running right
	ani->Add(10044);
	ani->Add(10045);
	ani->Add(10046);
	animations->Add(604, ani);

	ani = new CAnimation(50);//Fire running left
	ani->Add(10054);
	ani->Add(10055);
	ani->Add(10056);
	animations->Add(605, ani);

	ani = new CAnimation(20);//Raccoon running right
	ani->Add(10064);
	ani->Add(10065);
	ani->Add(10066);
	animations->Add(606, ani);

	ani = new CAnimation(20);//Raccoon running left
	ani->Add(10074);
	ani->Add(10075);
	ani->Add(10076);
	animations->Add(607, ani);



	ani = new CAnimation(100);		// brick
	ani->Add(20001);
	animations->Add(701, ani);

	ani = new CAnimation(300);		// Goomba walk
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(801, ani);

	ani = new CAnimation(1000);		// Goomba dead
	ani->Add(30003);
	animations->Add(802, ani);

	ani = new CAnimation(100);		//KoopaTroopa walk
	ani->Add(40001);
	ani->Add(40002);
	animations->Add(901, ani);

	ani = new CAnimation(1000);		//KoopaTroopa dead
	ani->Add(40003);
	animations->Add(902, ani);

	mario = new CMario();
	mario->AddAnimation(400);		// idle right big
	mario->AddAnimation(401);		// idle left big
	mario->AddAnimation(402);		// idle right small
	mario->AddAnimation(403);		// idle left small
	mario->AddAnimation(404);		//idle right fire
	mario->AddAnimation(405);		//idle left fire
	mario->AddAnimation(406);		//idle right raccoon
	mario->AddAnimation(407);		//idle left raccoon

	mario->AddAnimation(500);		// walk right big
	mario->AddAnimation(501);		// walk left big
	mario->AddAnimation(502);		// walk right small
	mario->AddAnimation(503);		// walk left small
	mario->AddAnimation(504);		//walk right fire
	mario->AddAnimation(505);		//walk left fire 
	mario->AddAnimation(506);		//walk right raccoon
	mario->AddAnimation(507);		//walk left raccoon

	mario->AddAnimation(600);		//run right big
	mario->AddAnimation(601);		//run left big
	mario->AddAnimation(602);		//run right small
	mario->AddAnimation(603);		//run left small
	mario->AddAnimation(604);		//run right fire
	mario->AddAnimation(605);		//run left fire
	mario->AddAnimation(606);		//run right raccoon
	mario->AddAnimation(607);		//run left raccoon

	mario->AddAnimation(650);		//brake right big
	mario->AddAnimation(651);		//brake left big
	mario->AddAnimation(652);		//brake right small
	mario->AddAnimation(653);		//brake left small
	mario->AddAnimation(654);		//brake right fire
	mario->AddAnimation(655);		//brake left fire
	mario->AddAnimation(656);		//brake right raccoon
	mario->AddAnimation(657);		//brake left raccoon


	mario->AddAnimation(699);		// die

	mario->SetPosition(50.0f, 0);
	objects.push_back(mario);
	// and Goombas 
	//for (int i = 0; i < 4; i++)
	//{
	//	goomba = new CGoomba();
	//	goomba->AddAnimation(801);
	//	goomba->AddAnimation(802);
	//	goomba->SetPosition(200 + i * 60, 135);
	//	goomba->SetState(GOOMBA_STATE_WALKING);
	//	objects.push_back(goomba);
	//}
	////KoopaTroopa
	koopatroopa = new CKoopaTroopa();
	koopatroopa->AddAnimation(901);
	koopatroopa->AddAnimation(902);
	for (int i = 0; i < 5; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation(701);
		brick->SetPosition(100.0f + i * 60.0f, 74.0f);
		objects.push_back(brick);

		brick = new CBrick();
		brick->AddAnimation(701);
		brick->SetPosition(100.0f + i * 60.0f, 90.0f);
		objects.push_back(brick);

		brick = new CBrick();
		brick->AddAnimation(701);
		brick->SetPosition(84.0f + i * 60.0f, 90.0f);
		objects.push_back(brick);
	}


	for (int i = 0; i < 30; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation(701);
		brick->SetPosition(0 + i * 16.0f, 150);
		objects.push_back(brick);
	}


}
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);
	/*koopatroopa->GetPosition(cx, cy);*/

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}

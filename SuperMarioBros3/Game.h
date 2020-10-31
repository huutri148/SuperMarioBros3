#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include "Scene.h"
#include <dinput.h>
#include "Utils.h"
#define KEYBOARD_BUFFER_SIZE 1024
/*
Abstract class to define keyboard event handlers
*/


class Game
{
	static Game* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8			di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8	didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; };
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	void Draw( int nx, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	float GetCamY() { return this->cam_y; };
	float GetCamX() { return this->cam_x; };
	static Game* GetInstance();

	int GetScreenWidth() { return screen_width; };
	int GetScreenHeight() { return screen_height; };


	/*void Load(LPCWSTR gameFile);*/
	//LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	//void SwitchScene(int scene_id);
	~Game();
};



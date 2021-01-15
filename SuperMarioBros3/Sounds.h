#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <dsound.h>
#include "dsutil.h"
#include "Define.h"



class Sounds
{
public:
	LPDIRECTSOUND8        g_pDS;
	//-----Các loại âm thanh
	LPDIRECTSOUNDBUFFER DSBuffer[30];

	Sounds();
	~Sounds();
	void LoadSound(HWND hWnd);
	bool InitDirectSound(HWND hwnd);
	void shutdownDirectSound();
	LPDIRECTSOUNDBUFFER LoadWaveToSoundBuffer(LPTSTR wavFilename);
	void playSound(int ID);
	void playSoundLoop(int ID);
	void stopSound(int ID);
};


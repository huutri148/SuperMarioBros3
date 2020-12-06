#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class Sprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(int nx,float x, float y, int alpha = 255, float translateX = 0.0f,
		float translateY = 0.0f);
	void Draw(int nx,int ny, float x, float y, int alpha = 255);

};

typedef Sprite* LPSPRITE;

/*
	Manage sprite database
*/
class Sprites
{
	static Sprites* __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);
	LPSPRITE& operator[](int id) { return sprites[id]; }
	void Clear();
	static Sprites* GetInstance();
};

///*
//	Sprite animation
//*/
//class CAnimationFrame
//{
//	LPSPRITE sprite;
//	DWORD time;
//
//public:
//	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
//	DWORD GetTime() { return time; }
//	LPSPRITE GetSprite() { return sprite; }
//};
//
//typedef CAnimationFrame* LPANIMATION_FRAME;
//
//class CAnimation
//{
//	DWORD lastFrameTime;
//	int defaultTime;
//	int currentFrame;
//	vector<LPANIMATION_FRAME> frames;
//public:
//	CAnimation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
//	void Add(int spriteId, DWORD time = 0);
//	void Render(float x, float y, int alpha = 255);
//};
//
//typedef CAnimation* LPANIMATION;
//
//class CAnimations
//{
//	static CAnimations* __instance;
//
//	unordered_map<int, LPANIMATION> animations;
//
//public:
//	void Add(int id, LPANIMATION ani);
//	LPANIMATION Get(int id);
//
//	static CAnimations* GetInstance();
//};


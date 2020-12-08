#include "Sprites.h"
#include "Game.h"

Sprite::Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

Sprites* Sprites::__instance = NULL;

Sprites* Sprites::GetInstance()
{
	if (__instance == NULL) __instance = new Sprites();
	return __instance;
}

void Sprite::Draw(/*int accordingcam*/int nx,float x, float y, int alpha, float translateX, float translateY)
{
	Game* game = Game::GetInstance();
	game->Draw(nx,x,y, texture, left, top, right, bottom, alpha, translateX, translateY);
}
void Sprite::Draw(/*int accordingcam*/int nx,int ny, float x, float y, int alpha)
{
	Game* game = Game::GetInstance();
	game->Draw(nx,ny, x, y, texture, left, top, right, bottom, alpha);
}


void Sprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
	
	DebugOut(L"[InFo] sprite addded: %d, %d, %d, %d, %d \n",
		id, left, top, right, bottom);
}

LPSPRITE Sprites::Get(int id)
{
	return sprites[id];
}


void Sprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}
	sprites.clear();
}



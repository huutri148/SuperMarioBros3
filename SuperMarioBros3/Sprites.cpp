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

void Sprite::Draw(/*int accordingcam*/int nx,float x, float y, int alpha)
{
	Game* game = Game::GetInstance();
	game->Draw(nx,x,y, texture, left, top, right, bottom, alpha);
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


//void CAnimation::Add(int spriteId, DWORD time)
//{
//	int t = time;
//	if (time == 0) t = this->defaultTime;
//
//	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
//	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
//	frames.push_back(frame);
//}
//
//void CAnimation::Render(float x, float y, int alpha)
//{
//	DWORD now = GetTickCount();
//	if (currentFrame == -1)
//	{
//		currentFrame = 0;
//		lastFrameTime = now;
//	}
//	else
//	{
//		DWORD t = frames[currentFrame]->GetTime();
//		if (now - lastFrameTime > t)
//		{
//			currentFrame++;
//			lastFrameTime = now;
//			if (currentFrame == frames.size()) currentFrame = 0;
//		}
//
//	}
//
//	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
//}
//
//CAnimations* CAnimations::__instance = NULL;
//
//CAnimations* CAnimations::GetInstance()
//{
//	if (__instance == NULL) __instance = new CAnimations();
//	return __instance;
//}
//
//void CAnimations::Add(int id, LPANIMATION ani)
//{
//	animations[id] = ani;
//}
//
//LPANIMATION CAnimations::Get(int id)
//{
//	return animations[id];
//}
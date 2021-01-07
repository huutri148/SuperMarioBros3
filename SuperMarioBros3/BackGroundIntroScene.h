#pragma once
#include"GameObject.h"
#include"Sprites.h"
class BackGroundIntroScene : public GameObject
{
	LPSPRITE scrollingCurtain;
	LPSPRITE ground;
	vector<LPSPRITE> backGrounds;
	int indexBackGround;
	DWORD shakingTime = 0;

	
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual int GetWidth() { return 0; };
	BackGroundIntroScene();
	DWORD beginSection2Time;
	DWORD section3Time = 0;
	
	bool isDoneSection3 = false;
	bool isBeginSection3 = false;
	bool isDoneSection1 = false;
	bool isBeginSection1 = false;
	bool isBeginSection2 = false;
};


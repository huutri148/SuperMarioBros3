#pragma once
#include"GameObject.h"
class WorldMapPanel : public GameObject
{
	int sceneId;
	DWORD changeStateTime = 0;
public:
	int possibleLeft = 0, possibleRight = 0, possibleTop = 0, possibleBottom = 0;
	int type;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable) {};
	virtual int GetWidth() { return 0; };
	virtual void Render(float translateX = 0, float translateY = 0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	WorldMapPanel(int left,int top,int right, int bottom,int sceneId = 0,int type = 0);
	int GetSceneId() { return sceneId; };
	void SetState(int state);
};


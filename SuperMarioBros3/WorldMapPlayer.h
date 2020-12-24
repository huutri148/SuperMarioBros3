#pragma once
#include"GameObject.h"
#include"WorldMapPanel.h"
#include"WorldMapBush.h"

class WorldMapPlayer : public GameObject
{

	//Todo: chuyển thông tin qua player
	WorldMapPanel* nearestPanel;
	int form;
public:
	WorldMapPanel* currentPanel;

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable) {};
	virtual void Render(float translateX = 0, float translateY = 0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	void Down();
	void Up();
	void Left();
	void Right();

	void FindNearestPanel();

	WorldMapPlayer();

};


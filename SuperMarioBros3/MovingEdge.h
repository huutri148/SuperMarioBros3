#pragma once
#include"GameObject.h"
#include"Define.h"
class MovingEdge : public GameObject
{
	float stopX;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable) {};
	virtual void SetState(int state);
	virtual int GetWidth() { return 0; };
	MovingEdge(float startX,float y, float stopX)
	{
		x = startX;
		this->y = y;
		this->stopX = stopX;
		this->SetState(MOVING_EDGE_STATE_MOVING);
	}

};


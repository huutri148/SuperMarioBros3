#pragma once
#include "GameObject.h"
#include"Define.h"
class Pipe :public GameObject
{
	
public:
	int type;
	Pipe(int type = PIPE_NORMAL_TYPE) { this->type = type; };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual int GetWidth() { return PIPE_BBOX_WIDTH; };
};


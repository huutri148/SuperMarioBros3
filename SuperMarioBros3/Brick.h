#pragma once
#include "GameObject.h"
#include "Define.h"



class Brick : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b,bool isEnable);
};




class InvisibleBrick : public GameObject
{
private:
	int type;
public:
	InvisibleBrick(int _type = 0) { type = _type; };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	int GetType() { return type; };
};
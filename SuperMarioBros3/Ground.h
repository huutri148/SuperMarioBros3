#pragma once
#include"GameObject.h"
#include"Define.h"
class Ground : public GameObject
{
	float width, height;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
};
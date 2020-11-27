#pragma once
#include"GameObject.h"
#include"Define.h"
class Ground : public GameObject
{
	int width, height;
public:
	/*Ground(float x, float y, float width, float height);*/
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
};
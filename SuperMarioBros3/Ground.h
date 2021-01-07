#pragma once
#include"GameObject.h"
#include"Define.h"
class Ground : public GameObject
{
	float width, height;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual int GetWidth() { return (int)width; };
	Ground(float width = 16.0f, float height = 16.0f)
	{
		this->width = width;
		this->height = height;
	};
};
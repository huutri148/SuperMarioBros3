#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Animations.h"
#include "Sprites.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CollisionEvent(float t, float nx,
		float ny, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny; 
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a,
		LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};



class GameObject
{
public:

	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;
	int ny;

	int state;
	bool isEnable;

	DWORD dt;

	//vector<LPANIMATION> animations;
	LPANIMATION_SET animation_set;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetEnable(bool enable) { this->isEnable = enable; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	bool IsEnable() { return this->isEnable; };

	int GetState() { return this->state; }
	virtual void SetState(int state) { this->state = state; }
	
	void RenderBoundingBox();
	
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);

	/*void AddAnimation(int aniId);*/
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	GameObject() ;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom,bool isEnable) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;


	~GameObject();
};


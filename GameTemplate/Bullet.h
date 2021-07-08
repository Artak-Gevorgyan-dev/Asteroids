#pragma once
#include "Vector2.h"
class Bullet
{
public:
	Vector2 direction;
	Vector2 position;
	~Bullet();
	float time;
	bool needToDestroy(float dt);
};


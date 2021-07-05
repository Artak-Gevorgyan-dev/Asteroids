#pragma once
class Vector2 {
public :
	float x;
	float y;
	Vector2(float x, float y);
	Vector2();
	Vector2 operator + (const Vector2& other);
	Vector2 operator - (const Vector2& other);
	Vector2 operator / (const float& other);
	void normalize();
};


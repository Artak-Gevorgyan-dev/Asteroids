#include "Vector2.h"
#include <math.h>

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}
Vector2::Vector2() {
	this->x = 0;
	this->y = 0;
}
Vector2 Vector2::operator + (const Vector2& other) {
	Vector2 tempVec;
	tempVec.x =this->x + other.x;
	tempVec.y = this->y + other.y;
    return tempVec;
}
Vector2 Vector2::operator - (const Vector2& other) {
	Vector2 tempVec;
	tempVec.x = this->x - other.x;
	tempVec.y = this->y - other.y;
	return tempVec;
}
Vector2 Vector2::operator / (const float& other) {
	Vector2 tempVec;
	tempVec.x = this->x / other;
	tempVec.y = this->y / other;
	return tempVec;
}
void Vector2::normalize()
{
	float lenght =sqrt(this->x* this->x + this->y * this->y);
	this->x = this->x / lenght;
	this->y = this->y / lenght;
}
float Vector2::lenght()
{
	float lenght = sqrt(this->x * this->x + this->y * this->y);
	return lenght;
}



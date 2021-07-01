#include "Vector2.h"

Vector2::Vector2(int x, int y) {
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

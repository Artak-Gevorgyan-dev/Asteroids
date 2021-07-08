#include "Bullet.h"
float time=0;
bool Bullet::needToDestroy(float dt) {
	time += dt;
	if (time > 2) {
		
		//this->~Bullet();
		return true;
		
	}
	return false;
}
Bullet::~Bullet() {

}

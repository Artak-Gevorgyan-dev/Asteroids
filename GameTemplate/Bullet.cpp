#include "Bullet.h"
float time=0;
bool Bullet::needToDestroy(float &dt) {
	time += dt;
	if (time > 2) {
		return true;
		
	}
	return false;
}
Bullet::~Bullet() {

}
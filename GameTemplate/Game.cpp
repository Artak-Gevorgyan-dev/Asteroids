#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>  
#include <windows.h>
#include <math.h>
#include "Vector2.h"
#include "Bullet.h"
#include <vector>
#include <sstream>
#include "Asteroid.h"
#include <ctime>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

using namespace std;

void drawSquare(int size, int pointX, int pointY);
int clamp(int value, int min, int max);
void drawTriangle(int size, Vector2 point);
void drawline(Vector2 point, Vector2 point2, uint32_t color);
Vector2 rotateRound(Vector2 point, Vector2 pivot, float angle);
void thrusterFlame(int size, Vector2 point);
void setPixel(int x, int y, uint32_t color);
void shoot();
bool OnButtonDown(int button);
void drawAstroids(Vector2 position, float size);
void spawnAstroids(Vector2 position, float size);
void drawSquareOutline(Vector2 position, float size);
float mirror(float value, int min, int max);

float shipX = 0;
float shipY = 0;
int a = 500;
float shipAngle=0;
float speed= 0.01;
Vector2 forwardDir = Vector2();
Vector2 currentforwardDir = Vector2();
Vector2 shipMid = Vector2();
vector<Bullet*> shootedBullets = vector<Bullet*>();
vector<Asteroid*> spawnedAsteroids = vector<Asteroid*>();

void DBOut( const int line)
{
	std::wostringstream os_;
	os_ << line ;
	OutputDebugStringW(os_.str().c_str());
}
// initialize game data in this function
void initialize()
{
	srand(time(0));
	shipY = SCREEN_WIDTH / 2;
	shipX = SCREEN_HEIGHT / 2;
	for (int i = 0; i < 5; i++)
	{
		spawnAstroids(Vector2(rand()% SCREEN_HEIGHT, rand() % SCREEN_HEIGHT), rand() % 20 +5);
		//DBOut(rand()% SCREEN_HEIGHT);
	}

}




// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();
  if (is_key_pressed(VK_UP)) {
	  // forwardDir.normalize();
	  currentforwardDir = forwardDir + currentforwardDir;
	  // currentforwardDir.normalize();



  }
  if (is_key_pressed(VK_RIGHT)) {

 	  shipAngle -= 5 * dt;
  }else if (is_key_pressed(VK_LEFT)) {
	  shipAngle += 5 * dt;
  }
  if (OnButtonDown('A')) {
	  shoot();
		
  }
    
  shipX += speed * dt * currentforwardDir.x;
  shipY += speed * dt * currentforwardDir.y;
  shipX = mirror(shipX, 0, SCREEN_HEIGHT);
  shipY = mirror(shipY, 0, SCREEN_WIDTH);
  if (spawnedAsteroids.size()==0) {
	  for (int i = 0; i < 5; i++)
	  {
		  spawnAstroids(Vector2(rand() % SCREEN_HEIGHT, rand() % SCREEN_HEIGHT), rand() % 20+10);
	  }
  }
  for (int i = 0;i< shootedBullets.size(); i++) {

	  if (shootedBullets[i]->needToDestroy(dt)) {

  		  shootedBullets[i]->time;

		  delete shootedBullets[i];

		  shootedBullets.erase(shootedBullets.begin() + i);
	  }
  }
  for (int i = 0; i < spawnedAsteroids.size(); i++) {

	  for (int j = 0; j < shootedBullets.size(); j++)
	  {
		  bool a = shootedBullets[j]->position.x < spawnedAsteroids[i]->position.x + spawnedAsteroids[i]->size ;
		  bool b = shootedBullets[j]->position.x > spawnedAsteroids[i]->position.x - spawnedAsteroids[i]->size ;
		  bool c = shootedBullets[j]->position.y < spawnedAsteroids[i]->position.y + spawnedAsteroids[i]->size ;
		  bool d = shootedBullets[j]->position.y > spawnedAsteroids[i]->position.y - spawnedAsteroids[i]->size ;
		  if (a && b && c && d) {
			  
			  if (spawnedAsteroids[i]->size > 20) {
				  spawnAstroids(spawnedAsteroids[i]->position, spawnedAsteroids[i]->size-10);
				  spawnAstroids(spawnedAsteroids[i]->position, spawnedAsteroids[i]->size-10);
			  }

			  delete spawnedAsteroids[i];

			  spawnedAsteroids.erase(spawnedAsteroids.begin() + i);
			  
			  delete shootedBullets[j];

			  shootedBullets.erase(shootedBullets.begin() + j);
			  break;
			
		}
	  }
  }
}



float mirror(float value,int min,int max) {
	max--;
	if (value > max)
		return min + (value - max);
	else if (value < min)
		return max + (value + min);
	return value;
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  drawTriangle(10,  Vector2(shipX,shipY));
  
  if (is_mouse_button_pressed(0)) 
  {
	  drawSquare(100, get_cursor_y(), get_cursor_x());
	  
  }
  if (is_key_pressed(VK_UP)) {
	  thrusterFlame(10, shipMid);
  }

  for (auto bullet : shootedBullets) {
	  bullet->position.x += 0.01 * bullet->direction.x;
	  bullet->position.y += 0.01 * bullet->direction.y;
	  bullet->position.x = mirror(bullet->position.x, 0, SCREEN_HEIGHT);;
	  bullet->position.y = mirror(bullet->position.y, 0, SCREEN_WIDTH);;
	  drawSquare(5, bullet->direction.x + bullet->position.x, bullet->direction.y + bullet->position.y);
  }
  for (auto asteroid : spawnedAsteroids) {
	  asteroid->position.x += 0.01 * asteroid->direction.x * asteroid->speed;
	  asteroid->position.y += 0.01 * asteroid->direction.y * asteroid->speed;
	  asteroid->position.x = mirror(asteroid->position.x, 0, SCREEN_HEIGHT);
	  asteroid->position.y = mirror(asteroid->position.y, 0, SCREEN_WIDTH);
	  drawSquareOutline(Vector2(asteroid->direction.x + asteroid->position.x, asteroid->direction.y + asteroid->position.y), asteroid->size);
  }

}
bool previousPress = false;
bool pressed = false;

void spawnAstroids(Vector2 position, float size) {
	auto asteroid = new Asteroid();
	double randomValueX =((double)rand()/RAND_MAX) * 2 -1;

	double randomValueY = ((double)rand() / RAND_MAX) * 2 - 1;
	asteroid->direction = Vector2(randomValueX, randomValueY);
	asteroid->position = position;
	asteroid->speed = rand()%10+1;
	asteroid->size = size;
	spawnedAsteroids.emplace_back(asteroid);
}

void drawAstroids(Vector2 position,float size) {

}

void drawSquareOutline(Vector2 position, float size) {
	Vector2 leftD = Vector2(position.x - 1* size, position.y - 1 * size);
	Vector2 rightD = Vector2(position.x + 1 * size, position.y - 1 * size);
	Vector2 leftU = Vector2(position.x - 1 * size, position.y + 1 * size);
	Vector2 rightU = Vector2(position.x + 1 * size, position.y + 1 * size);


	buffer[(int)position.x][(int)position.y] = 0xFF0000 ;

	drawline(leftD, rightD, 0xEEEEEE);
	drawline(rightD, rightU, 0xEEEEEE);
	drawline(rightU, leftU, 0xEEEEEE);
	drawline(leftU, leftD, 0xEEEEEE);
}

bool OnButtonDown(int button)
{
	pressed = is_key_pressed(button);
	if (previousPress != pressed)
	{
		previousPress = pressed;
		if (pressed)
		{
			return true;
		}
	}

	return false;
}

void shoot() {
	
	auto bullet = new Bullet();
	bullet->direction = forwardDir;
	bullet->position = shipMid;
	shootedBullets.emplace_back(bullet);
}

// free game data in this function
void finalize()
{

}


int clamp(int value, int min, int max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;
	return value;
}

void setPixel(int x, int y, uint32_t color) {
	x = mirror(x, 0, SCREEN_HEIGHT);
	y = mirror(y, 0, SCREEN_WIDTH);

	if (x<0 || x>SCREEN_HEIGHT)
	{
		DBOut('X');
		return;
	}
	if (y<0 || x>SCREEN_WIDTH)
	{
		DBOut('Y');
		return;
	}

	buffer[x][y] = color;
}

void thrusterFlame(int size, Vector2 point) {

	size = size / 1.2;

	Vector2 right = Vector2(size + point.x, 0 + point.y);
	Vector2 left = Vector2(0 + point.x, -size + point.y);
	Vector2 upper = Vector2(0 + point.x, size + point.y);



	right = rotateRound(right, point, shipAngle);
	left = rotateRound(left, point, shipAngle);
	upper = rotateRound(upper, point, shipAngle);

	drawline(left, right, 0xFFFFFF);
	drawline(left, upper, 0xFFFFFF);
	drawline(upper, right, 0xFFFFFF);
}

void drawTriangle(int size, Vector2 point) {
		//for (size_t i = 0; i < size; i++)
		//{
		//	for (size_t j = (size/2)-i/2.5; j < size / 2+i/2.5; j++)
		//	{
		//		int x = i + point.x;
		//		int y = j + point.y;
		//		x = clamp(x, 0, SCREEN_HEIGHT);
		//		y = clamp(y, 0, SCREEN_WIDTH);
		//		buffer[x][y] = 0xFF0000;
		//	}
		//}


	Vector2 point2 = Vector2(point.x, point.y + 50);

	Vector2 direction = point2 - point;

	float m = -(direction.x / direction.y);

	float b = m * point.x + point.y;

	Vector2 right = Vector2(6 * size + point.x, 3 * size + point.y);
	Vector2 left = Vector2(6 * size + point.x, 1 * size + point.y);
	Vector2 upper = Vector2(2 * size + point.x, 2 * size + point.y);
	shipMid = right + (left - right) / 2;

	right = rotateRound(right, shipMid, shipAngle);
	left = rotateRound(left, shipMid, shipAngle);
	upper = rotateRound(upper, shipMid, shipAngle);

	drawline(left, right, 0xFFFFFF);
	drawline(left, upper, 0xFFFFFF);
	drawline(upper, right, 0xFFFFFF);

	setPixel(upper.x, upper.y, 0xFFFFFF);

	setPixel(shipMid.x, shipMid.y, 0xFFFFFF);
	forwardDir = upper - shipMid;

}

Vector2 rotateRound(Vector2 point, Vector2 pivot, float angle) {

	Vector2 finalPoint;
	float s = sin(angle);
	float c = cos(angle);

	finalPoint.x = point.x - pivot.x;
	finalPoint.y = point.y - pivot.y;

	float xnew = finalPoint.x * c - finalPoint.y * s;
	float ynew = finalPoint.x * s + finalPoint.y * c;

	finalPoint.x = xnew + pivot.x;
	finalPoint.y = ynew + pivot.y;
	
	
	return finalPoint;
}


void drawSquare(int size, int pointX, int pointY) {
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			int x = j + pointX - size / 2;
			int y = i + pointY - size / 2;
			x = clamp(x, 0, SCREEN_HEIGHT);
			y = clamp(y, 0, SCREEN_WIDTH);
			setPixel(x, y, 0xFFFFFF);


		}
	}
}
void drawline(Vector2 point, Vector2 point2, uint32_t color)
{
	int dx = point2.x - point.x;
	int dy = point2.y - point.y;

	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	float Xinc = dx / (float)steps;
	float Yinc = dy / (float)steps;

	float X = point.x;
	float Y = point.y;
	for (int i = 0; i <= steps; i++)
	{


		setPixel((int)round(X), (int)round(Y), color);
		X += Xinc;          
		Y += Yinc;       
	}
}

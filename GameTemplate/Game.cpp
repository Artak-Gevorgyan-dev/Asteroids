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
void spawnAsteroids(Vector2 position, float size);
void drawSquareOutline(Vector2 position, float size);
float mirror(float value, int min, int max);
void restartGame();
uint32_t getPixel(int x, int y);
void DisplayNumber(int number, Vector2 point, int size);

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
int lifes=3;
int scores = 0;
bool restarted = true;
bool invariable;
float invariableTime;

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
	//set ship startPoint
	shipY = SCREEN_WIDTH / 2;
	shipX = SCREEN_HEIGHT / 2;
	//spawn the asteroids
	for (int i = 0; i < 5; i++)
	{
		Vector2 pos =  Vector2(rand() % SCREEN_HEIGHT, rand() % SCREEN_HEIGHT);
		//if asteroid spwan point near the player get another point
		if (sqrt(pow(pos.x - shipX, 2) +
			pow(pos.y - shipY, 2) * 1.0)<300) {
			i--;
			continue;
		}
		spawnAsteroids(Vector2(rand()% SCREEN_HEIGHT, rand() % SCREEN_HEIGHT), rand() % 20 +5);
		//DBOut(rand()% SCREEN_HEIGHT);
	}

}



// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();

  //if lifes gone don't do anything
  if (lifes <= 0)
	  return;

  //get invariability for 1 second after respawn
  if (!restarted) {
	  invariable = true;
	  invariableTime += dt;
	  if (invariableTime > 1) {
		  invariableTime = 0;
		  restarted = true;
		  invariable = false;
	  }
		
  }

  //give acceleration to ship
  if (is_key_pressed(VK_UP)) {
	  currentforwardDir = forwardDir + currentforwardDir;
  }

  //rotate the ship
  if (is_key_pressed(VK_RIGHT)) {

 	  shipAngle -= 4 * dt;
  }else if (is_key_pressed(VK_LEFT)) {
	  shipAngle += 4 * dt;
  }
  if (OnButtonDown(VK_SPACE)) {
	  shoot();
  }
    
  //calculate ship position every frame
  shipX += speed * dt * currentforwardDir.x;
  shipY += speed * dt * currentforwardDir.y;
  shipX = mirror(shipX, 0, SCREEN_HEIGHT);
  shipY = mirror(shipY, 0, SCREEN_WIDTH);


  //if all asteroids was destroyed respawn more
  if (spawnedAsteroids.size()==0) {
	  for (int i = 0; i < 5; i++)
	  {
		  spawnAsteroids(Vector2(rand() % SCREEN_HEIGHT, rand() % SCREEN_HEIGHT), rand() % 20+10);
	  }
  }
  //destroy shooted bullets after 2 seconds
  for (int i = 0;i< shootedBullets.size(); i++) {

	  if (shootedBullets[i]->needToDestroy(dt)) {

  		  shootedBullets[i]->time;

		  delete shootedBullets[i];

		  shootedBullets.erase(shootedBullets.begin() + i);
	  }
  }
  //destroy asteroids if bullet get inside the asteroid
  for (int i = 0; i < spawnedAsteroids.size(); i++) {

	  for (int j = 0; j < shootedBullets.size(); j++)
	  {
		  bool a = shootedBullets[j]->position.x < spawnedAsteroids[i]->position.x + spawnedAsteroids[i]->size ;
		  bool b = shootedBullets[j]->position.x > spawnedAsteroids[i]->position.x - spawnedAsteroids[i]->size ;
		  bool c = shootedBullets[j]->position.y < spawnedAsteroids[i]->position.y + spawnedAsteroids[i]->size ;
		  bool d = shootedBullets[j]->position.y > spawnedAsteroids[i]->position.y - spawnedAsteroids[i]->size ;
		  if (a && b && c && d) {
			  scores++;
			  if (spawnedAsteroids[i]->size > 15) {
				  spawnAsteroids(spawnedAsteroids[i]->position, spawnedAsteroids[i]->size-10);
				  spawnAsteroids(spawnedAsteroids[i]->position, spawnedAsteroids[i]->size-10);
			  }

			  delete spawnedAsteroids[i];

			  spawnedAsteroids.erase(spawnedAsteroids.begin() + i);
			  //destroy bullet also
			  delete shootedBullets[j];
			 
			  shootedBullets.erase(shootedBullets.begin() + j);
			  break;
			
		}
	  }
  }
}


//return the reversed position after geting out of screen
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

	//displaying lifes on left upper corner
	DisplayNumber(lifes, Vector2(10, 10), 20);

	//displaying scores on right upper corner
	string scoresDisplay = to_string(scores);
	reverse(scoresDisplay.begin(), scoresDisplay.end());
	//calculating position of every number of the score
		for (size_t i = 1; i <= scoresDisplay.length(); i++)
		{
		
			DisplayNumber((int)(scoresDisplay[i-1] - '0'), Vector2(10, SCREEN_WIDTH - 30* i), 20);
		}


	//calculating position of the bullet
	for (auto bullet : shootedBullets) {
		bullet->position.x += 0.01 * bullet->direction.x;
		bullet->position.y += 0.01 * bullet->direction.y;
		bullet->position.x = mirror(bullet->position.x, 0, SCREEN_HEIGHT);;
		bullet->position.y = mirror(bullet->position.y, 0, SCREEN_WIDTH);;
		drawSquare(5, bullet->direction.x + bullet->position.x, bullet->direction.y + bullet->position.y);
	}
	//calculating position of the asteroid
	for (auto asteroid : spawnedAsteroids) {
		asteroid->position.x += 0.01 * asteroid->direction.x * asteroid->speed;
		asteroid->position.y += 0.01 * asteroid->direction.y * asteroid->speed;
		asteroid->position.x = mirror(asteroid->position.x, 0, SCREEN_HEIGHT);
		asteroid->position.y = mirror(asteroid->position.y, 0, SCREEN_WIDTH);
		drawSquareOutline(Vector2(asteroid->direction.x + asteroid->position.x, asteroid->direction.y + asteroid->position.y), asteroid->size);
	}
	//if lifes is 0 don't draw the ship
	if (lifes <= 0)
		return;
	if (is_key_pressed(VK_UP)) {
		thrusterFlame(10, shipMid);
	}
	//if(invariable)
	drawTriangle(10, Vector2(shipX, shipY));
}
bool previousPress = false;
bool pressed = false;

//spawning asteroid with random direction
void spawnAsteroids(Vector2 position, float size) {
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

//drawing rectangle with lines
void drawSquareOutline(Vector2 position, float size) {
	Vector2 leftD = Vector2(position.x - 1* size, position.y - 1 * size);
	Vector2 rightD = Vector2(position.x + 1 * size, position.y - 1 * size);
	Vector2 leftU = Vector2(position.x - 1 * size, position.y + 1 * size);
	Vector2 rightU = Vector2(position.x + 1 * size, position.y + 1 * size);

	drawline(leftD, rightD, 0xEEEEEE);
	drawline(rightD, rightU, 0xEEEEEE);
	drawline(rightU, leftU, 0xEEEEEE);
	drawline(leftU, leftD, 0xEEEEEE);
}

//return true only one frame then button was pressed
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

//creating the bullet and adding to array
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

//clamping the value
int clamp(int value, int min, int max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;
	return value;
}

//set pixel to buffer
void setPixel(int x, int y, uint32_t color) {
	x = mirror(x, 0, SCREEN_HEIGHT);
	y = mirror(y, 0, SCREEN_WIDTH);
	buffer[x][y] = color;
}
//get pixel from buffer
uint32_t getPixel(int x, int y) {
	return  buffer[x][y];
}

//drawing thruster flame of the ship
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
//converting int number to pixels for showing scores and lifes
void DisplayNumber(int number, Vector2 point,int size) {
	switch (number)
	{
	case 0:
		for (size_t i = 0; i <= size/2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if (i == 0 || i == size/2 || j ==0 || j == size) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
	break;
	case 1:
		for (size_t j = 0; j <= size; j++)
		{

				int x = j + point.x;
				int y = point.y;
				setPixel(x, y, 0x00FFFF);

		}
		break;
	case 2:
	for (size_t i = 0; i <= size/2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ((i == 0 && j>size/2) || (i == size/2 && j<size/2)||(j ==0)||(j==size)||( j ==size/2)) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;
	case 3:
		for (size_t i = 0; i <= size / 2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ((i == size / 2 && j < size ) || (j == 0) || (j == size) || (j == size / 2)) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;
	case 4:
		for (size_t i = 0; i <= size / 2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ((i == 0 && j < size/2) || (i == size/2) || (j == size / 2)) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;
	case 5:
		for (size_t i = 0; i <= size / 2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ((i == 0 && j < size / 2) || (i == size / 2 && j >size/2) || j==0 || j==size|| j == size/2) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;
	case 6:
		for (size_t i = 0; i <= size / 2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ((i == 0 ) || (i == size / 2 && j > size / 2) || j == 0 || j == size || j == size / 2) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;
	case 7:
		for (size_t i = 0; i <= size / 2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ( (i == size / 2 ) || j == 0 ) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;

	case 8:
		for (size_t i = 0; i <= size / 2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ((i == 0) || (i == size / 2 ) || j == 0 || j == size || j == size / 2) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;
	case 9:
		for (size_t i = 0; i <= size / 2; i++)
		{
			for (size_t j = 0; j <= size; j++)
			{
				if ((i == 0 && j<size/2) || (i == size / 2) || j == 0 || j == size || j == size / 2) {
					int x = j + point.x;
					int y = i + point.y;
					setPixel(x, y, 0x00FFFF);
				}

			}
		}
		break;
	default:
		break;

	}

}

//drawing the ship
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

	//setPixel(shipMid.x, shipMid.y, 0xFFFFFF);
	forwardDir = upper - shipMid;

	setPixel(shipMid.x, shipMid.y, 0x00FF00);
	setPixel(upper.x, upper.y, 0x00FF00);
	for (float i = 0; i < size + (left-right).lenght()/2; i++)
	{
		for (float j = size - forwardDir.lenght(); j < 0; j++)
		{
			int x = j + left.x;
			int y = i + left.y;
			Vector2 vec = Vector2(x, y);
			vec = rotateRound(vec, left, shipAngle);

			

			vec.x = mirror(vec.x, 0, SCREEN_HEIGHT);
			vec.y = mirror(vec.y, 0, SCREEN_WIDTH);
			if (getPixel(vec.x, vec.y) == 0xEEEEEE) {
				if (restarted) {
					restarted = false;
					restartGame();

					return;
				}
			}
		}
	}
}
//restart game when get hit by asteroids
void restartGame() {
	shipY = SCREEN_WIDTH / 2;
	shipX = SCREEN_HEIGHT / 2;
	currentforwardDir = Vector2();
	shipAngle = 0;
	lifes--;
	for (int i = 0; i < spawnedAsteroids.size(); i++) {
		delete spawnedAsteroids[i];

		spawnedAsteroids.erase(spawnedAsteroids.begin() + i);
	}
	for (int i = 0; i < 5; i++)
	{
		spawnAsteroids(Vector2(rand() % SCREEN_HEIGHT, rand() % SCREEN_HEIGHT), rand() % 20 + 5);
		//DBOut(rand()% SCREEN_HEIGHT);
	}
	
}
//rotate points around pivot
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

//drawing the full rectangle 
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
//draw the simple line 
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

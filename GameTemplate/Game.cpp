#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>  
//#include <debugapi.h>
#include <windows.h>
#include <math.h>
#include "Vector2.h"

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


float shipX = 0;
float shipY = 0;
int a = 500;
float shipAngle=0;
float speed=0;
Vector2 forwardDir = Vector2();
Vector2 currentforwardDir = Vector2();
Vector2 shipMid = Vector2();

// initialize game data in this function
void initialize()
{
	shipY = SCREEN_WIDTH / 2;
	shipX = SCREEN_HEIGHT / 2;
}




// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();
	if (is_key_pressed(VK_UP)) {
	  //forwardDir.normalize();
	  currentforwardDir = forwardDir+ currentforwardDir;
	  //currentforwardDir.normalize();
	  
	  speed = 0.01 * dt;
	 
  }
  if (is_key_pressed(VK_RIGHT)) {

	  shipAngle -= 5 * dt;
  }
  else if (is_key_pressed(VK_LEFT)) {
	  shipAngle += 5 * dt;
  }
  shipX += speed * currentforwardDir.x;
  shipY += speed * currentforwardDir.y;
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
	x = clamp(x, 0, SCREEN_HEIGHT);
	y = clamp(y, 0, SCREEN_WIDTH);
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

void drawTriangle(int size,Vector2 point) {
//	for (size_t i = 0; i < size; i++)
//	{
//		for (size_t j = (size/2)-i/2.5; j < size / 2+i/2.5; j++)
//		{
//			int x = i + pointX;
//			int y = j + pointY;
//			x = clamp(x, 0, SCREEN_HEIGHT);
//			y = clamp(y, 0, SCREEN_WIDTH);
//			buffer[x][y] = 0xFF0000;
//		}
//	}


	Vector2 point2 = Vector2(point.x , point.y + 50);

	Vector2 direction = point2 - point;

	float m =-(direction.x/direction.y);

	float b = m * point.x + point.y;

	Vector2 right = Vector2(6 * size + point.x, 3 * size + point.y);
	Vector2 left  = Vector2(6 * size + point.x, 1 * size + point.y);
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
			try {
				setPixel(x, y, 0xFF0000);
			}
			catch (...) {

			}

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

#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>  
//#include <debugapi.h>
#include <windows.h>
#include <math.h>

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
void drawTriangle(int size, int pointX, int pointY);
void drawline(int X0, int Y0, int X1, int Y1, uint32_t color);

float shipX = 0;
float shipY = 0;
int a = 500;

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
  if (is_key_pressed(VK_DOWN)) {

	  shipX += 100 * dt;
	  cout << "a";
  }else if (is_key_pressed(VK_UP)) {
	  shipX -= 100 * dt;
	  cout << "a";
  }


}


// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  //drawSquare(100, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2);
  drawTriangle(10, shipX,shipY);
  if (is_mouse_button_pressed(0)) 
  {
	  drawSquare(100, get_cursor_y(), get_cursor_x());
	  
  }
}



//wchar_t buffer[256];
//
//
//wsprintfW(buffer, L"UP %d\n", (1.0f * dt));
//
//
//OutputDebugStringW(buffer);

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

void drawTriangle(int size,int pointX,int pointY) {
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
	//drawline(0, 33/2, 0, 66/2, 0xFF0000);
	//drawline(50/2,48/2,0,33/2, 0x00FF00);
	//drawline(50/2, 48/2, 0, 66/2, 0x00FF00);

	drawline(6 * size + pointX, 1 * size + pointY, 6 * size + pointX, 3 * size + pointY, 0xFF0000);
	drawline(6 * size + pointX, 1 * size + pointY, 2 * size + pointX, 2 * size + pointY, 0x00FF00);
	drawline(2 * size + pointX, 2 * size + pointY, 6 * size + pointX, 3 * size + pointY, 0xFFFF00);
	
	//drawline(100,50,0,100, 0x0000FF);
	//drawline(100, 0, 100, 100, 0x0000FF);
}

void drawSquare(int size, int pointX, int pointY) {
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			//buffer[clamp(i + pointX - size / 2,0, SCREEN_HEIGHT)][clamp(j + pointY - size / 2,0,SCREEN_WIDTH)] = 0xFF0000;
			int x = j + pointX - size / 2;
			int y = i + pointY - size / 2;
			x = clamp(x, 0, SCREEN_HEIGHT);
			y = clamp(y, 0, SCREEN_WIDTH);
			try {
				buffer[x][y] = 0xFF0000;
			}
			catch (...) {

			}

		}
	}
}
void drawline(int X0, int Y0, int X1, int Y1, uint32_t color)
{
	int dx = X1 - X0;
	int dy = Y1 - Y0;

	// calculate steps required for generating pixels
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	// calculate increment in x & y for each steps
	float Xinc = dx / (float)steps;
	float Yinc = dy / (float)steps;

	// Put pixel for each step
	float X = X0;
	float Y = Y0;
	for (int i = 0; i <= steps; i++)
	{
		buffer[(int)round(X)][(int)round(Y)] = color;  // put pixel at (X,Y)
		X += Xinc;           // increment in x at each step
		Y += Yinc;           // increment in y at each step							 // generation step by step
	}

	//int dx, dy, p, x, y;

	//dx = x1 - x0;
	//dy = y1 - y0;

	//x = x0;
	//y = y0;

	//p = 2 * dy - dx;

	//while (x < x1 || y< y1)
	//{
	//	if (p >= 0)
	//	{
	//		buffer[x][y] = color;
	//		y = y + 1;
	//		p = p + 2 * dy - 2 * dx;
	//	}
	//	else
	//	{
	//		buffer[x][y] = color;
	//		p = p + 2 * dy;
	//	}
	//	if(dx !=0 )
	//		x = x + 1;
	//}
}

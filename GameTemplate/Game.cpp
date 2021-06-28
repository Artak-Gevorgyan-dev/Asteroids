#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>  
//#include <debugapi.h>
#include <windows.h>

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

int shipX = 0;
int shipY = 0;
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
	  a += (1.0f * dt);
	  wchar_t buffer[256];
	  wsprintfW(buffer, L"DOWN %d\n", (1.0f * dt));

	  LPCWSTR debug = buffer;
	  OutputDebugStringW(debug);
  }

  if (is_key_pressed(VK_UP)) {
	  a -= (1.0f* dt);

	  wchar_t buffer[256];


	  wsprintfW(buffer, L"UP %d\n", (1.0f * dt));


	  OutputDebugStringW(buffer);
  }


}


// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
  //drawSquare(100, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2);
  drawTriangle(30, a,shipY);
  if (is_mouse_button_pressed(0)) {
	  //drawSquare(100, get_cursor_y(), get_cursor_x());
	  
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

void drawTriangle(int size,int pointX,int pointY) {
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = (size/2)-i/2.5; j < size / 2+i/2.5; j++)
		{
			int x = i + pointX;
			int y = j + pointY;
			x = clamp(x, 0, SCREEN_HEIGHT);
			y = clamp(y, 0, SCREEN_WIDTH);
			buffer[x][y] = 0xFF0000;
		}
	}
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

